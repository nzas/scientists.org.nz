<?php
//$Id$

/**
 * @file
 * Token module integration.
 */

/**
 * Implementation of hook_token_values().
 */
function role_expire_token_values($type, $object = NULL) {
  $values = array();
  if ($type == 'role_expire_role') {
    $values['rid']  = $object->rid;
    $values['name'] = $object->name;
    $tz = variable_get('date_default_timezone', 0);
    $values['expiry-date'] = format_date($object->expiry_timestamp, 'small', '', $tz);
    $values += token_get_date_token_values($object->expiry_timestamp, 'expiry-date-');
    $values['nearend-days'] = $object->nearend_days;
  }
  return $values;
}

/**
 * Implementation of hook_token_list().
 */
function role_expire_token_list($type = 'all') {
  if ($type == 'role_expire_role' || $type == 'all') {
    $tokens['role_expire_role']['rid']  = t('Role ID');
    $tokens['role_expire_role']['name'] = t('Role name');
    $tokens['role_expire_role']['expiry-date'] = t('Role expiration date');
    $tokens['role_expire_role'] += token_get_date_token_info(t('Role expiration'), 'expiry-date-');
    $tokens['role_expire_role']['nearend-days'] = t('Number of days before role expiration');
  }
  return $tokens;
}