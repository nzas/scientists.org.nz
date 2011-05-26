<?php
  //$Id: scs.admin.inc,v 1.1.2.1.2.2 2010/06/10 19:24:49 coworks Exp $
  
  /*
   * @file
   * Select Drupal content to create a newsletter
   */

  /*
   * Admin settings form
   */
  function scs_admin_settings_form() {
  	$form = array();
  	
  	$form['scs_content_types'] = array(
  	  '#type' => 'checkboxes',
  	  '#title' => t('Content types'),
  	  '#description' => t('Select the content types that must be avaiable for node selection'),
  	  '#options' => _scs_get_node_types(),
  	  '#default_value' => variable_get('scs_content_types', array()),
  	);
  	
  	$form['scs_format'] = array(
  	  '#type' => 'radios',
  	  '#title' => t('Newsletter format'),
  	  '#description' => t('Select the format of the newsletters you want to send'),
  	  '#options' => array('plain' => t('Plain'), 'html' => t('HTML')),
  	  '#default_value' => variable_get('scs_format', 'plain'),
  	);
  	
  	return system_settings_form($form);
  }
  
  /*
   * Return avaiable node types in option list
   */
  function _scs_get_node_types() {
  	$options = array();
  	
  	foreach (node_get_types() as $name=>$type) {
  		$options[$name] = $type->name;
  	}
  	
  	return $options;
  }