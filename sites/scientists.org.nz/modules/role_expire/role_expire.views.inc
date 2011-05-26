<?php
// $Id: role_expire.views.inc,v 1.8 2010/07/02 17:03:20 stewsnooze Exp $

/**
 * @file
 * Role Expire Views hooks
 *
 * Views module integration with the role expire module
 */

/**
 * Implementation of hook_views_data().
 */
function role_expire_views_data() {
  $data['role_expire']['table']['group']  = t('User');

  $data['role_expire']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'inner',
    ),
    'role' => array(
    'left_field' => 'rid',
    'field' => 'rid',
    'type' => 'inner',    
    ),
  );
  $data['role_expire']['expiry_timestamp'] = array(
    'title' => t('Role expiration time'),
    'help' => t('Time and date the role will expire.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['role']['name'] = array(
    'title' => t('Role expiration name'),
    'group' => 'User',
    'help' => t('Name of the role that is associated with this role expiry. This allows you to
                see the role name that expires on a particular date.'),
    'relationship' => array(
      'label' => t('Role'),
      'base' => 'role',
      'base field' => 'name',
      'skip base' => array('role'),
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),

  );
  $data['role_expire']['rid'] = array(
    'title' => t('Role expiration Role ID'),
    'help' => t('Role ID in Role Expire.'),
    'group' => 'User',
    'relationship' => array(
      'label' => t('Role'),
      'base' => 'role',
      'base field' => 'rid',
      'skip base' => array('role'),
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
  );
  $data['role_expire']['uid'] = array(
    'title' => t('Role expiration User ID'),
    'help' => t('User ID in Role Expire.'),
    'group' => 'User',
    'relationship' => array(
      'label' => t('User'),
      'base' => 'user',
      'base field' => 'uid',
      'skip base' => array('user'),
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
  );
  
  return $data;
}