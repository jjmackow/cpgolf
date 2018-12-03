<?php

/**
 * @file
 * Contains the views ui tabs style plugin.
 */

/**
 * Style plugin to render each item in ui tabs
 *
 * @ingroup views_style_plugins
 */
class views_ui_tabs_plugin_style_tabs extends views_plugin_style {

  // Set default options
  function option_definition() {
    $options = parent::option_definition();
    $options['sortable'] = array('default' => FALSE);
    $options['tabs_create_text'] = array('default' => '+');
    return $options;
  }

  /**
   * Options form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // sortable option
    $form['sortable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Sortable tabs'),
      '#default_value' => $this->options['sortable'],
      '#description' => t('If enabled, the tabs can be dragged to reorder the view results. The user must have the "Can reorder tabs" permission.'),
    );

    $form['tabs_create_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Create tab text'),
      '#default_value' => $this->options['tabs_create_text'],
      '#description' => t('The text displayed on the create tab if a create url is provided.'),
    );

    // alter description title
    $form['grouping']['#description'] = t('Specify a field by which to group the records. A grouping field is <strong>required</strong> for the tabs to render correctly.');
  }

  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      debug('views_ui_tabs_plugin_style_tabs: Missing row plugin');
      return;
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.  Plugins may override this
    // method if they wish some other way of handling grouping.
    $output = '';
    $uses_row_plugin = $this->uses_row_plugin();
    foreach ($sets as $title => $records) {
      $rows[$title] = array();
      if ($uses_row_plugin) {
        foreach ($records as $row_index => $row) {
          $this->view->row_index = $row_index;
          $rows[$title][$row_index] = $this->row_plugin->render($row);
        }
      }
      else {
        $rows[$title][] = $records;
      }
    }
    unset($this->view->row_index);

    $output .= theme($this->theme_functions(),
     array(
       'view' => $this->view,
       'options' => $this->options,
       'rows' => $rows,
       'title' => '')
     );
    return $output;
  }

  /**
   * Validate style
   */
  function validate() {
    $errors = parent::validate();

    // results must be grouped
    if (empty($this->options['grouping'])) {
      $errors[] = t('Style @style requires a grouping field.', array('@style' => $this->definition['title']));
    }

    // sortable should be sorted by weight field
    if (!empty($this->options['sortable'])) {
/* @todo: check sort arguments - similar to draggable views *******************************/
      //$errors[] = t('Style @style requires a grouping field.', array('@style' => $this->definition['title']));
    }

    return $errors;
  }

}
