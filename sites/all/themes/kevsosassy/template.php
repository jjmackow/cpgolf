<?php
/**
 * @file
 * The primary PHP file for this theme.
 */
function kevsosassy_bugfix_custom_theme(){
    if('system/ajax' === current_path()){
       if(isset($_POST['ajax_page_state']['theme'])){
           $themes = list_themes();
           if(isset($themes[$_POST['ajax_page_state']['theme']]) && !empty($themes[$_POST['ajax_page_state']['theme']]->status)){
               return $_POST['ajax_page_state']['theme'];
           }
       }
    }
    return variable_get('theme_default', 'bartik');    
}

function kevsosassy_menu_tree(array $variables) {
  return '<ul class="nav navbar-nav">' . $variables['tree'] . '</ul>';
}

function kevsosassy_theme() {
  $items = array();
	
  $items['user_login'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'kevsosassy') . '/templates',
    'template' => 'user-login',
    'preprocess functions' => array(
       'kevsosassy_preprocess_user_login'
    ),
  );
  $items['user_register_form'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'kevsosassy') . '/templates',
    'template' => 'user-register-form',
    'preprocess functions' => array(
      'kevsosassy_preprocess_user_register_form'
    ),
  );
  $items['user_pass'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'kevsosassy') . '/templates',
    'template' => 'user-pass',
    'preprocess functions' => array(
      'kevsosassy_preprocess_user_pass'
    ),
  );
  return $items;
}

function kevsosassy_preprocess_user_login(&$vars) {
  $vars['intro_text'] = t('Please provide the follow:');
}

function kevsosassy_preprocess_user_register_form(&$vars) {
  $vars['intro_text'] = t('This is my super awesome reg form');
}

function kevsosassy_preprocess_user_pass(&$vars) {
  $vars['intro_text'] = t('This is my super awesome request new password form');
}
function kevsosassy_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => 'form-required',
    'title' => $t('This field is required.'),
  );
  return '<span' . drupal_attributes($attributes) . '></span>';
}
function getStrsBetween($s,$s1,$s2=false,$offset=0) {
    /*====================================================================
    Function to scan a string for items encapsulated within a pair of tags

    getStrsBetween(string, tag1, <tag2>, <offset>

    If no second tag is specified, then match between identical tags

    Returns an array indexed with the encapsulated text, which is in turn
    a sub-array, containing the position of each item.

    Notes:
    strpos($needle,$haystack,$offset)
    substr($string,$start,$length)

    ====================================================================*/

    if( $s2 === false ) { $s2 = $s1; }
    $result = array();
    $L1 = strlen($s1);
    $L2 = strlen($s2);

    if( $L1==0 || $L2==0 ) {
        return false;
    }

    do {
        $pos1 = strpos($s,$s1,$offset);

        if( $pos1 !== false ) {
            $pos1 += $L1;

            $pos2 = strpos($s,$s2,$pos1);

            if( $pos2 !== false ) {
                $key_len = $pos2 - $pos1;

                $this_key = substr($s,$pos1,$key_len);

                if( !array_key_exists($this_key,$result) ) {
                    $result[$this_key] = array();
                }

                $result[$this_key][] = $pos1;

                $offset = $pos2 + $L2;
            } else {
                $pos1 = false;
            }
        }
    } while($pos1 !== false );

    return $result;
}

// This is the function to call..
// parameters:
// 		html: the whole html document containing images
// 		imageSrcDelimiter: The ending character of the src string
//			either single quote: ' or double quote:"
// returns
//		string (filtered original html)
function removeImageQueryStrings($html, $imageSrcDelimiter='"'){
	// image types

	$fileTypes = array(".jpg", ".png", ".gif");

	// for each image type...
	foreach($fileTypes as $fileType){

		// make sure all image file type instances are the same case (lower)
		$html = str_replace(strtoupper($fileType), strtolower($fileType), $html);

		// find all instances of the query strings in the html string
		// 	make sure we use lower case since we just converted the text to use it
		$imageQueryStringArray = getStrsBetween($html, strtolower($fileType), $imageSrcDelimiter);

		// create the original tag including file type and closing quote...
		foreach($imageQueryStringArray as $queryString => $offset){

			$replacementSource = strtolower($fileType) . $queryString . $imageSrcDelimiter;
			$replacementTarget = strtolower($fileType) . $imageSrcDelimiter;

			$html = str_replace($replacementSource, $replacementTarget, $html);
			$html = str_replace($replacementSource, $replacementTarget, $html);
		}
	}
	return $html;
}