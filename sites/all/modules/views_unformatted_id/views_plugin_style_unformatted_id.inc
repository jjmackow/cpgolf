<?php

/**
 * @file
 * Contains the unformatted list with css row ids style plugin.
 */

/**
 * Style plugin to render rows one after another with no
 * decorations just additional CSS row ids.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_unformatted_id extends views_plugin_style_default {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['row_id'] = array('default' => '[view_name]-[current_display]-[row_id]');
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $form['row_id'] = array(
      '#title' => t('Row id'),
      '#description' => t('CSS ID for rows.'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row_id'],
    );

    if ($this->uses_fields()) {
      $form['row_id']['#description'] .= ' ' . t('You may use field tokens from as per the "Replacement patterns" used in "Rewrite the output of this field" for all fields.');
      $form['row_id']['#description'] .= '<p>' . t('[view_name], [current_display] and [row_id] are available as additional tokens.') . '</p>';
    }
    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  function get_row_id($row_index) {
    // Add some additional tokens to help to create real unique ids.
    $this->row_tokens[$row_index]['[view_name]'] = $this->view->name;
    $this->row_tokens[$row_index]['[current_display]'] = $this->view->current_display;
    $this->row_tokens[$row_index]['[row_id]'] = $row_index;
    $id = $row_index;
    if (!empty($this->options['row_id'])) {
      if ($this->uses_fields() && $this->view->field) {
        $id = strip_tags($this->tokenize_value($this->options['row_id'], $row_index));
      }
      else {
        $id = $this->options['row_id'];
      }
    }
    return drupal_html_id($id);
  }

}
