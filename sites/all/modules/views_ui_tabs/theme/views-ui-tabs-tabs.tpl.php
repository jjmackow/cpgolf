<?php
/**
 * @file views-ui-tabs-tabs.tpl.php
 *
 * @ingroup views_templates
 */
?>
<?php
  $o = '';
  if (!empty($rows)) {
    $o .= '<div id="' . $html_container_id . '">';
      // tabs anchors
      $o .= '<div class="tabs-ui-nav-wrapper">';
      $o .= '<ul>';
        foreach ($tabs_html_ids as $tab_title => $tab_html_id) {
          $o .= '<li><a href="#' . $tab_html_id . '">' . $tab_title . '</a></li>';
        }
      $o .= '</ul>';
      $o .= '</div>';

      // tabs content
      
      $o .= '<div class="views-ui-tabs-content-wrapper">';
      foreach ($rows as $tab_title => $tab_rows) {
        $o .= '<div id="' . $tabs_html_ids[$tab_title] . '">';
          if (!empty($tab_rows)) {
            foreach ($tab_rows as $id => $row) {
              $o .= '<div' . (!empty($classes_array[$id]) ? ' class="' . $classes_array[$id] . '"' : '') . '>';
                $o .= $row;
              $o .= '</div>';
            }
          }
        $o .= '</div>';
      }
      $o .= '</div>';
    $o .= '</div>';
  }

  echo $o;
?>
