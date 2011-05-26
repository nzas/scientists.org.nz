<?php
// $Id$

/**
 * @file
 *   User field handler for privacy-enabled fields.
 *
 * @see views_handler_field_user.inc
 */

class profile_privacy_views_handler_field_user extends views_handler_field_user {

  function option_definition() {
    $options = parent::option_definition();
    $options += _profile_privacy_views_options_definition();
    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    _profile_privacy_views_options_form($form, $form_state, $this);
  }


  function render($values) {
    if (_profile_privacy_views_field_access($values->uid, $this->definition['profile_privacy_field_name'], $this)) {
      return parent::render($values);
    }
    elseif (drupal_strlen($this->options['profile_privacy_replacement_text'])) {
      return '<span class="private">' . filter_xss_admin($this->options['profile_privacy_replacement_text']) . '</span>';
    }
  }


  function access() {
    return _profile_privacy_views_field_access(NULL,  $this->definition['profile_privacy_field_name'], $this);
  }

}
<?php
// $Id$

/**
 * @file
 *   User field handler for privacy-enabled fields.
 *
 * @see views_handler_field_user.inc
 */

class profile_privacy_views_handler_field_user extends views_handler_field_user {

  function option_definition() {
    $options = parent::option_definition();
    $options += _profile_privacy_views_options_definition();
    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    _profile_privacy_views_options_form($form, $form_state, $this);
  }


  function render($values) {
    if (_profile_privacy_views_field_access($values->uid, $this->definition['profile_privacy_field_name'], $this)) {
      return parent::render($values);
    }
    elseif (drupal_strlen($this->options['profile_privacy_replacement_text'])) {
      return '<span class="private">' . filter_xss_admin($this->options['profile_privacy_replacement_text']) . '</span>';
    }
  }


  function access() {
    return _profile_privacy_views_field_access(NULL,  $this->definition['profile_privacy_field_name'], $this);
  }

}
<?php
// $Id$

/**
 * @file
 *   User field handler for privacy-enabled fields.
 *
 * @see views_handler_field_user.inc
 */

class profile_privacy_views_handler_field_user extends views_handler_field_user {

  function option_definition() {
    $options = parent::option_definition();
    $options += _profile_privacy_views_options_definition();
    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    _profile_privacy_views_options_form($form, $form_state, $this);
  }


  function render($values) {
    if (_profile_privacy_views_field_access($values->uid, $this->definition['profile_privacy_field_name'], $this)) {
      return parent::render($values);
    }
    else {
      return '<span class="private">' . t($this->options['profile_privacy_replacement_text']) . '</span>';
    }
  }


  function access() {
    return _profile_privacy_views_field_access(NULL,  $this->definition['profile_privacy_field_name'], $this);
  }

}
