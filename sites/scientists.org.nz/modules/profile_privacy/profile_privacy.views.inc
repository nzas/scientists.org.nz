<?php
// $Id$

/**
 * @file
 *   Adds views support for profile_privacy
 */

/**
 * @todo
 *  - Better theme integration.
 *    - Class for private fields even when they're rendered. So that
 *      they can stand out to admins, for example.
 */

/**
 * Implementation of hook_views_data_alter()
 */
function profile_privacy_views_data_alter(&$data) {
  $keys = array_keys($data);
  foreach ($data as $key => $value) {
    preg_match('/^profile_values_(.*)/', $key, $matches);
    if (isset($matches[1])) {
      $new_handler = 'profile_privacy_' . $data[$key]['value']['field']['handler'];
      $data[$key]['value']['field']['handler'] = $new_handler;
      $data[$key]['value']['field']['profile_privacy_field_name'] = $matches[1];
    }
  }
}


/**
 * Implementation of hook_views_handlers()
 */
function profile_privacy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'profile_privacy') .'/views/handlers',
    ),
    'handlers' => array(
      'profile_privacy_views_handler_field' => array(
        'parent' => 'views_handler_field',
      ),

      'profile_privacy_views_handler_field_user' => array(
        'parent' => 'views_handler_field_user',
      ),

      'profile_privacy_views_handler_field_boolean' => array(
        'parent' => 'views_handler_field_boolean',
      ),

      'profile_privacy_views_handler_field_profile_date' => array(
        'parent' => 'views_handler_field_profile_date',
      ),

      'profile_privacy_views_handler_field_markup' => array(
        'parent' => 'views_handler_field_markup',
      ),

      'profile_privacy_views_handler_field_url' => array(
        'parent' => 'views_handler_field_url',
      ),

      'profile_privacy_views_handler_field_profile_list' => array(
        'parent' => 'views_handler_field_profile_list',
      ),
    ),
  );
}


/**
 * Helper function to add field options.
 *
 * @return
 *   Options array.
 *
 * @see views_handler_field
 */

function _profile_privacy_views_options_definition() {
  $options['profile_privacy_replacement_text'] = array('default' => '', 'translatable' => TRUE);
  $options['profile_privacy_show_hidden'] = array('default' => FALSE);
  return $options;
}


/**
 * Helper function to add field options form elements.
 *
 * @param $form
 *   Forms API form definition.
 *
 * @param $form_state
 *   Forms API form state definition.
 *
 * @param $handler
 *   Views field handler object.  Handler whose options_form() method
 *   called this function.
 *
 * @see profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_options_form(&$form, &$form_state, $handler) {
  $form['profile_privacy_replacement_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Privacy replacement text'),
      '#default_value' => $handler->options['profile_privacy_replacement_text'],
      '#description' => t('If the field is hidden from the viewer, display this text instead.'),
    );
  if (user_access('administer users')) {
      $form['profile_privacy_show_hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show field even if configured as "Hidden profile field"'),
        '#default_value' => $handler->options['profile_privacy_show_hidden'],
        '#description' => t('Use with caution.  If checked, the field will display for non-admin users even if it is set as "Hidden profile field" in the Profile module configuration.  Users cannot override this setting or alter the information displayed.'),
      );
  }

}

/**
 * Helper function to determine user's access to view a profile field
 * within a view.
 *
 * @param $uid
 *   User ID to which the field being tested belongs.
 *
 * @param $field_alias
 *   Profile module internal name for the field being tested.
 *
 * @param $handler
 *   Views field handler object.  Handler whose render() method
 *   called this function.
 *
 * @return boolean
 *   TRUE if user has access to the field, FALSE otherwise.
 *
 * @see profile_privacy_get_user_field_privacy(),
 *   profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_field_access($uid=0, $field_alias, $handler) {
  // Administrators get full access.
  $administrator = user_access('administer users');
  if ($administrator) {
    return TRUE;
  }
  else {
    // Check profile privacy value
    $profile_fields = profile_privacy_get_fields();

    /**
     * User cannot override fields only visible to admin / modules,
     * even if it was formerly accessible to the user.
     *
     * As a side-effect, this test accomplishes the whole-field access
     * test used by the field handler's access() method.  Any field set as
     * PROFILE_HIDDEN will not appear at all to users without
     * 'admin users' access, UNLESS the "Show even if hidden" option
     * is set for the field in the specific view.
     */
    if ($profile_fields[$field_alias]->visibility != PROFILE_HIDDEN) {
      /**
       * If $uid = NULL, then this is a field-wide check.  Can allow
       * field-wide access at this point because it's not a PROFILE_HIDDEN
       * field.
       *
       * Because the default for the $uid parameter is 0, the field-wide
       * access check must explicitly specify NULL.
       */
      if (is_null($uid)) {
        return TRUE;
      }

      // For any other fields, the user can control privacy.
      $private_fields = profile_privacy_get_user_privacy($uid);
      $access =
        /**
         * Rule summary:
         * Access allowed if
         *   User has explicitly disabled privacy -OR-
         *   profile module setting is VISIBLE and user has not overridden.
         *
         * Only PROFILE_PUBLIC_LISTING is accepted as VISIBLE, so that
         * fields that are marked PROFILE_PUBLIC are hidden unless user
         * explicitly overrides.
         *
         * Logic Summary:
         *   - For PROFILE_PUBLIC_LISTING fields, default to public.
         *     Profile module: "content shown on profile page and on
         *     member list pages."
         *   - For PROFILE_PUBLIC fields, default to private.
         *     Profile module: "content shown on profile page but not
         *     used on member list pages."
         *   - For PROFILE_PRIVATE fields, default to private.
         *     Profile module: "content only available to privileged
         *     users."
         *   - User can override any of the above.
         *
         *   This means that for the purposes of Views,
         *   PROFILE_PUBLIC & PROFILE_PRIVATE are treated the same.
         */
        isset($private_fields[$field_alias]) && $private_fields[$field_alias] == 0 ||
        ($profile_fields[$field_alias]->visibility == PROFILE_PUBLIC_LISTINGS &&
          empty($private_fields[$field_alias])) // Encompases: !isset($private_fields[$field_alias]) || $private_fields[$field_alias] === 0
      ;
      return $access;
    }
    /**
     * Field was set as PROFILE_HIDDEN in Profile module config, so check
     * whether it's overridden in the view.
     *
     * Type-specific check used for added protection against false
     * positives from data corruption.
     */
    return ($handler->options['profile_privacy_show_hidden'] === (int)TRUE);
  }
}


/*
  (select) views_handler_field
  (textfields) views_handler_field_user
  (checkboxes) views_handler_field_boolean
  (date) views_handler_field_profile_date
  (textareas) views_handler_field_markup
  (url) views_handler_field_url
  (freeform lists) views_handler_field_prerender_list
*/
<?php
// $Id$

/**
 * @file
 *   Adds views support for profile_privacy
 */

/**
 * @todo
 *  - Better theme integration.
 *    - Class for private fields even when they're rendered. So that
 *      they can stand out to admins, for example.
 */

/**
 * Implementation of hook_views_data_alter()
 */
function profile_privacy_views_data_alter(&$data) {
  $keys = array_keys($data);
  foreach ($data as $key => $value) {
    preg_match('/^profile_values_(.*)/', $key, $matches);
    if (isset($matches[1])) {
      $new_handler = 'profile_privacy_' . $data[$key]['value']['field']['handler'];
      $data[$key]['value']['field']['handler'] = $new_handler;
      $data[$key]['value']['field']['profile_privacy_field_name'] = $matches[1];
    }
  }
}


/**
 * Implementation of hook_views_handlers()
 */
function profile_privacy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'profile_privacy') .'/views/handlers',
    ),
    'handlers' => array(
      'profile_privacy_views_handler_field' => array(
        'parent' => 'views_handler_field',
      ),

      'profile_privacy_views_handler_field_user' => array(
        'parent' => 'views_handler_field_user',
      ),

      'profile_privacy_views_handler_field_boolean' => array(
        'parent' => 'views_handler_field_boolean',
      ),

      'profile_privacy_views_handler_field_profile_date' => array(
        'parent' => 'views_handler_field_profile_date',
      ),

      'profile_privacy_views_handler_field_markup' => array(
        'parent' => 'views_handler_field_markup',
      ),

      'profile_privacy_views_handler_field_url' => array(
        'parent' => 'views_handler_field_url',
      ),

      'profile_privacy_views_handler_field_profile_list' => array(
        'parent' => 'views_handler_field_profile_list',
      ),
    ),
  );
}


/**
 * Helper function to add field options.
 *
 * @return
 *   Options array.
 *
 * @see views_handler_field
 */

function _profile_privacy_views_options_definition() {
  $options['profile_privacy_replacement_text'] = array('default' => '', 'translatable' => TRUE);
  $options['profile_privacy_show_hidden'] = array('default' => FALSE);
  return $options;
}


/**
 * Helper function to add field options form elements.
 *
 * @param $form
 *   Forms API form definition.
 *
 * @param $form_state
 *   Forms API form state definition.
 *
 * @param $handler
 *   Views field handler object.  Handler whose options_form() method
 *   called this function.
 *
 * @see profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_options_form(&$form, &$form_state, $handler) {
  $form['profile_privacy_replacement_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Privacy replacement text'),
      '#default_value' => $handler->options['profile_privacy_replacement_text'],
      '#description' => t('If the field is hidden from the viewer, display this text instead.'),
    );
  if (user_access('administer users')) {
      $form['profile_privacy_show_hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show field even if configured as "Hidden profile field"'),
        '#default_value' => $handler->options['profile_privacy_show_hidden'],
        '#description' => t('Use with caution.  If checked, the field will display for non-admin users even if it is set as "Hidden profile field" in the Profile module configuration.  Users cannot override this setting or alter the information displayed.'),
      );
  }

}

/**
 * Helper function to determine user's access to view a profile field
 * within a view.
 *
 * @param $uid
 *   User ID to which the field being tested belongs.
 *
 * @param $field_alias
 *   Profile module internal name for the field being tested.
 *
 * @param $handler
 *   Views field handler object.  Handler whose render() method
 *   called this function.
 *
 * @return boolean
 *   TRUE if user has access to the field, FALSE otherwise.
 *
 * @see profile_privacy_get_user_field_privacy(),
 *   profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_field_access($uid=0, $field_alias, $handler) {
  // Administrators get full access.
  $administrator = user_access('administer users');
  if ($administrator) {
    return TRUE;
  }
  else {
    // Check profile privacy value
    $profile_fields = profile_privacy_get_fields();

    /**
     * User cannot override fields only visible to admin / modules,
     * even if it was formerly accessible to the user.
     *
     * As a side-effect, this test accomplishes the whole-field access
     * test used by the field handler's access() method.  Any field set as
     * PROFILE_HIDDEN will not appear at all to users without
     * 'admin users' access, UNLESS the "Show even if hidden" option
     * is set for the field in the specific view.
     */
    if ($profile_fields[$field_alias]->visibility != PROFILE_HIDDEN) {
      /**
       * If $uid = NULL, then this is a field-wide check.  Can allow
       * field-wide access at this point because it's not a PROFILE_HIDDEN
       * field.
       *
       * Because the default for the $uid parameter is 0, the field-wide
       * access check must explicitly specify NULL.
       */
      if (is_null($uid)) {
        return TRUE;
      }

      // For any other fields, the user can control privacy.
      $private_fields = profile_privacy_get_user_privacy($uid);
      $access =
        /**
         * Rule summary:
         * Access allowed if
         *   User has explicitly disabled privacy -OR-
         *   profile module setting is VISIBLE and user has not overridden.
         *
         * Only PROFILE_PUBLIC_LISTING is accepted as VISIBLE, so that
         * fields that are marked PROFILE_PUBLIC are hidden unless user
         * explicitly overrides.
         *
         * Logic Summary:
         *   - For PROFILE_PUBLIC_LISTING fields, default to public.
         *     Profile module: "content shown on profile page and on
         *     member list pages."
         *   - For PROFILE_PUBLIC fields, default to private.
         *     Profile module: "content shown on profile page but not
         *     used on member list pages."
         *   - For PROFILE_PRIVATE fields, default to private.
         *     Profile module: "content only available to privileged
         *     users."
         *   - User can override any of the above.
         *
         *   This means that for the purposes of Views,
         *   PROFILE_PUBLIC & PROFILE_PRIVATE are treated the same.
         */
        isset($private_fields[$field_alias]) && $private_fields[$field_alias] == 0 ||
        ($profile_fields[$field_alias]->visibility == PROFILE_PUBLIC_LISTINGS &&
          empty($private_fields[$field_alias])) // Encompases: !isset($private_fields[$field_alias]) || $private_fields[$field_alias] === 0
      ;
      return $access;
    }
    /**
     * Field was set as PROFILE_HIDDEN in Profile module config, so check
     * whether it's overridden in the view.
     *
     * Type-specific check used for added protection against false
     * positives from data corruption.
     */
    return ($handler->options['profile_privacy_show_hidden'] === (int)TRUE);
  }
}


/*
  (select) views_handler_field
  (textfields) views_handler_field_user
  (checkboxes) views_handler_field_boolean
  (date) views_handler_field_profile_date
  (textareas) views_handler_field_markup
  (url) views_handler_field_url
  (freeform lists) views_handler_field_prerender_list
*/
<?php
// $Id$

/**
 * @file
 *   Adds views support for profile_privacy
 */

/**
 * @todo
 *  - Better theme integration.
 *    - Class for private fields even when they're rendered. So that
 *      they can stand out to admins, for example.
 */

/**
 * Implementation of hook_views_data_alter()
 */
function profile_privacy_views_data_alter(&$data) {
  $keys = array_keys($data);
  foreach ($data as $key => $value) {
    preg_match('/^profile_values_(.*)/', $key, $matches);
    if (isset($matches[1])) {
      $new_handler = 'profile_privacy_' . $data[$key]['value']['field']['handler'];
      $data[$key]['value']['field']['handler'] = $new_handler;
      $data[$key]['value']['field']['profile_privacy_field_name'] = $matches[1];
    }
  }
}


/**
 * Implementation of hook_views_handlers()
 */
function profile_privacy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'profile_privacy') .'/views/handlers',
    ),
    'handlers' => array(
      'profile_privacy_views_handler_field' => array(
        'parent' => 'views_handler_field',
      ),

      'profile_privacy_views_handler_field_user' => array(
        'parent' => 'views_handler_field_user',
      ),

      'profile_privacy_views_handler_field_boolean' => array(
        'parent' => 'views_handler_field_boolean',
      ),

      'profile_privacy_views_handler_field_profile_date' => array(
        'parent' => 'views_handler_field_profile_date',
      ),

      'profile_privacy_views_handler_field_markup' => array(
        'parent' => 'views_handler_field_markup',
      ),

      'profile_privacy_views_handler_field_url' => array(
        'parent' => 'views_handler_field_url',
      ),

      'profile_privacy_views_handler_field_profile_list' => array(
        'parent' => 'views_handler_field_profile_list',
      ),
    ),
  );
}


/**
 * Helper function to add field options.
 *
 * @return
 *   Options array.
 *
 * @see views_handler_field
 */

function _profile_privacy_views_options_definition() {
  $options['profile_privacy_replacement_text'] = array('default' => '', 'translatable' => TRUE);
  $options['profile_privacy_show_hidden'] = array('default' => FALSE);
  return $options;
}


/**
 * Helper function to add field options form elements.
 *
 * @param $form
 *   Forms API form definition.
 *
 * @param $form_state
 *   Forms API form state definition.
 *
 * @param $handler
 *   Views field handler object.  Handler whose options_form() method
 *   called this function.
 *
 * @see profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_options_form(&$form, &$form_state, $handler) {
  $form['profile_privacy_replacement_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Privacy replacement text'),
      '#default_value' => $handler->options['profile_privacy_replacement_text'],
      '#description' => t('If the field is hidden from the viewer, display this text instead.'),
    );
  if (user_access('administer users')) {
      $form['profile_privacy_show_hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show field even if configured as "Hidden profile field"'),
        '#default_value' => $handler->options['profile_privacy_show_hidden'],
        '#description' => t('Use with caution.  If checked, the field will display for non-admin users even if it is set as "Hidden profile field" in the Profile module configuration.  Users cannot override this setting or alter the information displayed.'),
      );
  }

}

/**
 * Helper function to determine user's access to view a profile field
 * within a view.
 *
 * @param $uid
 *   User ID to which the field being tested belongs.
 *
 * @param $field_alias
 *   Profile module internal name for the field being tested.
 *
 * @param $handler
 *   Views field handler object.  Handler whose render() method
 *   called this function.
 *
 * @return boolean
 *   TRUE if user has access to the field, FALSE otherwise.
 *
 * @see profile_privacy_get_user_field_privacy(),
 *   profile_privacy_views_handler_field_user
 */
function _profile_privacy_views_field_access($uid=0, $field_alias, $handler) {
  // Administrators get full access.
  $administrator = user_access('administer users');
  if ($administrator) {
    return TRUE;
  }
  else {
    // Check profile privacy value
    $profile_fields = profile_privacy_get_fields();

    /**
     * User cannot override fields only visible to admin / modules,
     * even if it was formerly accessible to the user.
     *
     * As a side-effect, this test accomplishes the whole-field access
     * test used by the field handler's access() method.  Any field set as
     * PROFILE_HIDDEN will not appear at all to users without
     * 'admin users' access, UNLESS the "Show even if hidden" option
     * is set for the field in the specific view.
     */
    if ($profile_fields[$field_alias]->visibility != PROFILE_HIDDEN) {
      /**
       * If $uid = NULL, then this is a field-wide check.  Can allow
       * field-wide access at this point because it's not a PROFILE_HIDDEN
       * field.
       *
       * Because the default for the $uid parameter is 0, the field-wide
       * access check must explicitly specify NULL.
       */
      if (is_null($uid)) {
        return TRUE;
      }

      // For any other fields, the user can control privacy.
      $private_fields = profile_privacy_get_user_privacy($uid);
      $access =
        /**
         * Rule summary:
         * Access allowed if
         *   User has explicitly disabled privacy -OR-
         *   profile module setting is VISIBLE and user has not overridden.
         *
         * Only PROFILE_PUBLIC_LISTING is accepted as VISIBLE, so that
         * fields that are marked PROFILE_PUBLIC are hidden unless user
         * explicitly overrides.
         *
         * Logic Summary:
         *   - For PROFILE_PUBLIC_LISTING fields, default to public.
         *     Profile module: "content shown on profile page and on
         *     member list pages."
         *   - For PROFILE_PUBLIC fields, default to private.
         *     Profile module: "content shown on profile page but not
         *     used on member list pages."
         *   - For PROFILE_PRIVATE fields, default to private.
         *     Profile module: "content only available to privileged
         *     users."
         *   - User can override any of the above.
         *
         *   This means that for the purposes of Views,
         *   PROFILE_PUBLIC & PROFILE_PRIVATE are treated the same.
         */
        isset($private_fields[$field_alias]) && $private_fields[$field_alias] == 0 ||
        ($profile_fields[$field_alias]->visibility == PROFILE_PUBLIC_LISTINGS &&
          empty($private_fields[$field_alias])) // Encompases: !isset($private_fields[$field_alias]) || $private_fields[$field_alias] === 0
      ;
      return $access;
    }
    /**
     * Field was set as PROFILE_HIDDEN in Profile module config, so check
     * whether it's overridden in the view.
     *
     * Type-specific check used for added protection against false
     * positives from data corruption.
     */
    return ($handler->options['profile_privacy_show_hidden'] === (int)TRUE);
  }
}


/*
  (select) views_handler_field
  (textfields) views_handler_field_user
  (checkboxes) views_handler_field_boolean
  (date) views_handler_field_profile_date
  (textareas) views_handler_field_markup
  (url) views_handler_field_url
  (freeform lists) views_handler_field_prerender_list
*/