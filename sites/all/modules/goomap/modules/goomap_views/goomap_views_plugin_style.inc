<?php

/**
 * @file
 * Extension of the Views Plugin Style for Goomap
 */

class goomap_views_plugin_style extends views_plugin_style {

  var $map_items = array();

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    foreach (goomap_default_options() as $key => $value) {
      $options[$key] = array('default' => $value);
    }
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get list of fields in this view & flag available geodata fields
    $handlers = $this->display->handler->get_handlers('field');

    $fields_data = array();
    $fields_data_all = array();
    foreach ($handlers as $field_id => $handler) {
      $fields_data_all[$field_id] = $handler->ui_name();
      if (!empty($handler->field_info['type']) && $handler->field_info['type'] == 'geofield') {
        $fields_data[$field_id] = $handler->ui_name();
      }
    }
    // Check whether we have a geofield we can work with
    if (!count($fields_data)) {
      $form['error'] = array(
        '#markup' => t('Please add at least one geofield to the view'),
      );
      return;
    }
    $form['data_source'] = array(
      '#type' => 'select',
      '#title' => t('Data Source'),
      '#description' => t('Which field contains geodata?'),
      '#options' => $fields_data,
      '#default_value' => $this->options['data_source'],
      '#required' => TRUE,
    );
    $form = goomap_settings_form($form, $this->options);

    /*
    foreach($this->options as $key => $value) {
      if(isset($form[$key])) {
        $form[$key]['#default_value'] = $this->options[$key];
      }
    }
    */

    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }

    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    if (!empty($options)) {
      $output = '<p>' . t('The following tokens are available. If you would like to have the characters \'[\' and \']\' please use the html entity codes \'%5B\' or  \'%5D\' or they will get replaced with empty space.' . '</p>');
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . check_plain($value);
          }
          $output .= theme('item_list',
              array(
                'items' => $items,
                'type' => $type
              ));
        }
      }

      $form['token_help'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#value' => $output,
        '#id' => 'edit-options-token-help',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
    }
  }

  function tokenize_value($value, $row_index, $json = FALSE) {
    if (strpos($value, '[') !== FALSE || strpos($value, '!') !== FALSE || strpos($value, '%') !== FALSE) {
      // Row tokens might be empty, for example for node row style.
      $tokens = isset($this->row_tokens[$row_index]) ? $this->row_tokens[$row_index] : array();
      if (!empty($this->view->build_info['substitutions'])) {
        $tokens += $this->view->build_info['substitutions'];
      }
      if ($json) {
        foreach ($tokens as $key => $val) {
          if (is_string($val)) {
            $tokens[$key] = drupal_json_encode($val);
          }
        }
      }
      if ($tokens) {
        $value = strtr($value, $tokens);
      }
    }
    return $value;
  }

  /**
   * Allow the style to do stuff before each row is rendered.
   *
   * @param $result
   *   The full array of results from the query.
   */
  function pre_render($result) {
		parent::pre_render ( $result );
		// Render fields so tokens are available.
		$this->render_fields ( $this->view->result );
		$items = array ();
		foreach ( $result as $key1 => $field ) {
			if (! empty ( $this->options ['data_source'] )) {
				$field = ( array ) $field;
				// Remove field data.
				unset ( $field ['_field_data'] );
				foreach ( $field ['field_' . $this->options ['data_source']] as $key2 => $item ) {
					$newfield = $field;
					unset ( $newfield ['field_' . $this->options ['data_source']] );
					$newfield ['geodata'] = $field ['field_' . $this->options ['data_source']] [$key2] ['raw'];
					$newfield ['markerOptions'] = ( array ) json_decode ( filter_xss_admin ( $this->tokenize_value ( $this->options ['markerOptionsJson'], $key1, TRUE ) ) );
					$newfield ['infowindowContent'] = filter_xss_admin ( $this->tokenize_value ( $this->options ['infowindowContent'], $key1, FALSE ) );
					$this->map_items [] = $newfield;
				}
			}
		}
	}

  /**
   * Renders view.
   */
  function render() {
  	if(!empty($this->view->live_preview)){
  		if($this->view->use_ajax){
  			drupal_set_message('Ajax may not function correctly in the views preview.', 'warning');
  		}
  	}
    $settings = $this->options;
    return theme(
        'goomap', array(
          'mapid' => drupal_html_class('goomap-' . $this->view->name . '-' . $this->view->current_display),
          'settings' => $settings,
          'items' => $this->map_items,
        	'async' => TRUE,
        )
    );
  }
}
