<?php

/*
 * potential problems
 * @todo we use LANGUAGE_NONE everywhere...problem!
 * @todo using event CONTENT IS VIEWED slows down the node dramatically
 * @todo, what happens if we delete a node? The presets will not be deleted! hmm ...
 */

/*
 * INFO
 *
 * We let the user select the file scheme, so we could create the presets in the private file folder
 * eveb if the field is public. We do that to later add an access check on these presets.
 */


/**
 * @file
 * Rules code: actions for image styles.
 */

function rules_image_styles_rules_action_info() {

  $actions['rules_image_styles_action_generate'] = array(
    'label' => t('Pre generate image styles'),
    'group' => t('Image Styles'),

    'parameter' => array(
      'style_presets' => array(
        'type' => 'list<integer>',
        'label' => t('List of avaliable image styles'),
        'options list' => 'rules_image_styles_presets_list',
      ),
      'style_presets_schema' => array(
        'type' => 'list<integer>',
        'label' => t('Chose file Schema (ONLY CHOSE ONE)'),
        'options list' => 'rules_image_styles_schema_list',
      ),
      'style_presets_subfolder' => array(
        'type' => 'text',
        'label' => t('Subfolder'),
        'description' => t("<strong>No leading or trailing slash!</strong>. You may want to chose a subfolder where the styles are generated into. For example Use styles if you want them generated into sites/default/files/styles (sites/default/files is the private file basedir here)"),
        'optional' => TRUE,
      ),
      //pass in the node too, because we need to whole node object for token_replace
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
        'save' => TRUE,
      ),
    ),
  );

  return $actions;
}


/**
 * Generating the image styles
 *
 * We passed in the node object. Take all imagefields of the node and render all chosen presets
 */
function rules_image_styles_action_generate($selected_image_styles, $scheme ,$subfolder ,$node) {

  //get the nodes imagefields
  $node_imagefields = rules_image_styles_action_get_node_image_fields($node);

  //filter out the image styles we selected in the options
  $image_styles_to_use = array();
  $styles = image_styles();
  foreach ($styles as $name => $setting) {
    foreach ($selected_image_styles as $key => $selected_name) {
      if ($name == $selected_name) {
        $image_styles_to_use[$name] = $setting;
      }
    }
  }

//  /*
//  * inside the node object there are the fields like:
//  *  $node->field_texturbild['und'][0]['uri'] = public://TEXTURBILDER/Strom/dexter.png
//  * We check that and so we know the paths ... TEXTURBILD/Strom/dexter.png
//  */

//  //go through all image fields of a node and get the filepath+file of the images of that field
  foreach ($node_imagefields as $field_name => $settings) {
    //field could be empty, and there can be multiple fields
    if (!empty($node->{$field_name})) {
      foreach ($node->{$field_name}[LANGUAGE_NONE] as $key => $file) {

        //we can get the processed path here!
        $file_data = file_load($file['fid']);
        $file['uri'] = $file_data->uri;
        $images[$field_name][$key]['uri'] = $file['uri'];                                        //full uri
        $images[$field_name][$key]['target'] = file_uri_target($file['uri']);                    //everything after public://
        $images[$field_name][$key]['filename'] = basename($images[$field_name][$key]['target']);  //just the file.ext

        //ATTENTION, if there is no folder and the file is in public://file.ext, dirname will return ".", we need to kill "."
        $images[$field_name][$key]['path'] = (dirname($images[$field_name][$key]['target']) != ".")
                                            ? dirname($images[$field_name][$key]['target'])
                                            : '' ;
        $images[$field_name][$key]['scheme'] = reset($scheme);                    //uri scheme, e.g public, private .. reset because its an array
      }
    }
  }

//  //for each selected preset, generate ...
//  //@todo how to deal with already existing images???? does that matter because we use the node data. that should already be renamed if already exists.
    //@todo does it? NO oly images are kept somehow
  //@todo problem, flushing image styles DOES not create the new images so we need to do that by hand, check IF NEW IMAGE or IMAGE STYLE CHANGED


  //@todo, causes error on nodes that have NO image fields. so check for that

  foreach ($image_styles_to_use as $style_name => $data) {
    foreach ($images as $field_name => $image) {
      foreach ($image as $key => $filedata) {
        //@todo spamle here what values are
        //@todo what do we do i f image exists?? override?
        ($subfolder == '')
        ? $destination = $filedata['scheme'] . '://' . $style_name . '/' . $filedata['scheme'] . '/' . $filedata['path'] . '/' . $filedata['filename']
        : $destination = $filedata['scheme'] . '://' . $subfolder . '/' . $style_name . '/' . $filedata['scheme'] . '/' . $filedata['path'] . '/' . $filedata['filename'];
        image_style_create_derivative($data, $filedata['uri'], $destination);
      }
    }
  }

}


/***************************************************************************************************
 *      HELPER DUNCTIONS
 **************************************************************************************************/


/**
 * Helper function to build a select list for all image styles
 *
 * @return <array>
 *   An array with all image styles
 *
 * Why not use: image_style_options? -> keys will be saved 0 1 2 3..useless
 */
function rules_image_styles_presets_list() {
  $image_styles = array();
  foreach (image_styles() as $name => $settings) {
    $image_styles[$name] = $name;
  }
  return $image_styles;
}

/**
 * Helper function to build a select list for all image styles
 *
 * @return <array>
 *   An array with all image styles
 *
 * Why not use: image_style_options? -> keys will be saved 0 1 2 3..useless
 */
function rules_image_styles_schema_list() {
  $scheme = array(
    'public' => 'public',
    'private' => 'private',
  );

  return $scheme;
}

/**
 * Helper function to build a select list for all node types
 *
 * @return <array>
 *   An array with all node types
 */
/*function rules_image_styles_node_types() {
  $node_types = array();
  foreach (node_type_get_types() as $obj_name => $settings) {
    $node_types[$settings->type] = $settings->name;
  }
  return $node_types;
}*/

/*
 * Helper function to only get image_fields
 *
 * @params node
 *  a node object
 * @return <array>
 *   An array with all avaliable imagefields in all nodes
 */

function rules_image_styles_action_get_node_image_fields($node) {
  $image_fields = array();

  $node_fields = field_info_instances('node', $node->type);

  foreach ($node_fields as $name => $field_data) {
    //we check the widget here, is there a better solution? May this cause problems?
    if ($field_data['widget']['module'] == 'image') {
      $image_fields[$name] = $field_data;
    }
  }

  return $image_fields;
}
