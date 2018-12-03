<?php

/**
 * Implements template_preprocess_HOOK()
 */
function template_preprocess_views_ui_tabs_tabs(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];

  $vars['classes_array'] = array();
  $vars['classes'] = array();

  $vars['tabs_html_ids'] = array();
  $vars['html_container_id'] = drupal_html_id('views-ui-tabs');

  // Set up striping values.
  $count = 0;
  $max = count($rows);
  foreach ($rows as $set_id => $set_rows) {
    $tab_html_id = $vars['tabs_html_ids'][$set_id] = drupal_html_id('views-ui-tabs-tab');

    foreach ($set_rows as $id => $row) {
      $count++;

      $vars['classes'][$id][] = 'views-row';
      $vars['classes'][$id][] = 'views-row-' . $count;
      $vars['classes'][$id][] = 'views-row-' . ($count % 2 ? 'odd' : 'even');
      if ($count == 1) {
        $vars['classes'][$id][] = 'views-row-first';
      }
      if ($count == $max) {
        $vars['classes'][$id][] = 'views-row-last';
      }

      if ($row_class = $view->style_plugin->get_row_class($id)) {
        $vars['classes'][$id][] = $row_class;
      }

      // Flatten the classes to a string for each row for the template file.
      $vars['classes_array'][$id] = implode(' ', $vars['classes'][$id]);
    }
  }


  // merge view options and default values
  $vars['options'] =  $view->style_plugin->options + array(
    'selected' => 0, // ensure the first tab is selected
    'sortable' => FALSE,
    'tabs_create_text' => '+', // always set a default for create text
  );
}

/**
 * Implements template_process_HOOK()
 */
function template_process_views_ui_tabs_tabs(&$vars) {
  $options = (object) (!empty($vars['options']) ? $vars['options'] : array());

  // sanitize options
  $options->tabs_create_text = check_plain($options->tabs_create_text);

  // Add required js and css
  drupal_add_library('system', 'ui.tabs');
  if (!empty($options->sortable) && user_access('reorder views_ui_tabs tabs')) {
    drupal_add_library('system', 'ui.sortable');
  }

  // add views ui tabs js
  drupal_add_js(array('views_ui_tabs' => array($vars['html_container_id'] => $options)), 'setting');
  drupal_add_js(drupal_get_path('module', 'views_ui_tabs') . '/js/views_ui_tabs.js');
}
