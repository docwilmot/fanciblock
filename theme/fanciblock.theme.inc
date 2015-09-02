<?php
/**
 * @file
 * Theming functions for the fanciblock module.
 * 
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Template preprocess handler for 'fanciblock_content' theme.
 */
function template_preprocess_fanciblock_content(&$variables) {
  // Each fanciblock instance gets a unique id
  $fanciblock_id = &backdrop_static('fanciblock_id', 0);
  $variables['id'] = ++$fanciblock_id;
  $variables['i']  = 0;

  $variables['icon_size']	= NULL;
  $variables['icon_color']		= NULL;
  $variables['icon_bg_color']	= NULL;
  $variables['icon_border']		= NULL;
  $variables['icon_border_color']= NULL;
  $variables['icon_bg_border']	= NULL;
  $variables['box_bg_color']		= NULL;
  $variables['box_border_color']	= NULL;
  $variables['box_border']		= NULL;
  $variables['box_padding']		= NULL;

  // define width, height, bgcolor, bgimage for fanciblock
  if(!empty($variables['settings'])) {
    // Todo, this is ugly; add block type to settings array.
    $variables['fanciblock_type'] = $variables['settings']['fanciblock_type'];

    $variables['grid']				= $variables['settings']['grid'];
    $variables['icon_position']		= 'fanciblock-icon-position-'.$variables['settings']['icon-position'];
    $variables['icon_looks']			= 'fanciblock-icon-style-'.$variables['settings']['icon-style'];
    $variables['icon_size']			= 'fanciblock-icon-size-'.$variables['settings']['icon-size'];
    
    // Build icon border and background color
    if($variables['settings']['icon-style']	!= 'none') {
      // Build background color style.
      if($variables['settings']['icon-bg-color']	== '') {
        $variables['icon_bg_color']	= 'background-color:transparent;';
      } else {
        $variables['icon_bg_color']	= 'background-color:'.$variables['settings']['icon-bg-color'].';';
      }
      
      // If icon border is set, then build the whole sentence by joining the 
      // border width, style and color settings sentences.
      if($variables['settings']['icon-border']	== 1) {
        $variables['icon_border']	= 'border-width:';
        $variables['icon_border']	.= $variables['settings']['icon-border-width'] .'px;';
        $variables['icon_border']	.= 'border-style:';
        $variables['icon_border']	.= $variables['settings']['icon-border-style'].';';
        $variables['icon_border']	.= 'border-color:';
        $variables['icon_border']	.= $variables['settings']['icon-border-color'].';';
      }
    
      // Create icon style sentence by joining icon color and background border.
      $variables['icon_style']	= 'style="' . $variables['icon_bg_color'] . $variables['icon_border'] . '"'; 
    }

    // Build the box background.
    if($variables['settings']['box-bg-color'] == '') {
      $box_bg_color	= 'background-color:transparent;';
    } else {
      $box_bg_color	= 'background-color:'.$variables['settings']['box-bg-color'].';';
    }
    
    // Build the box border.
    if($variables['settings']['box-border'] == 1) {
      // Build a border color sentence.
      $box_border	= 'border-width:';
      $box_border	.= $variables['settings']['box-border-width'] .'px;';
      $box_border	.= 'border-style:';
      $box_border	.= $variables['settings']['box-border-style'].';';
      $box_border	.= 'border-color:';
      $box_border	.= $variables['settings']['box-border-color'].';';
    }
    
    // Build padding
    $box_padding		= 'padding:' . $variables['settings']['box-padding'] . 'px;';
    
    // Finally
    $variables['box_style'] = 'style ="';
    $variables['box_style'] .= $box_bg_color;
    $variables['box_style'] .= $box_padding;
    $variables['box_style'] .= $box_border;
    $variables['box_style'] .= '"';
    
    $variables['box_class'] = $variables['icon_position'] .' ' . $variables['icon_looks'] . ' ' . $variables['icon_size'];
  }
  
  //
  $variables['items'] = array();
  $i = 0;
  $item_number = count($variables['fanciblock_data']);
  dpm($variables);
  foreach ($variables['fanciblock_data'] as $box => $item ) {
    // Build column classes.
    // Add first, last, odd, even class.
    $i++;
    $variables['col_class'][0] = 'fanciblock-col';
    $variables['col_class'][1] = 'fanciblock-col-'.$i;
    if ($i == 1) { $variables['col_class'][2] = 'col-first';}
    if ($i == $item_number) {$variables['col_class'][2] = 'col-last';}
    if ($i % 2 == 0) {$variables['col_class'][4] = 'col-even';}
    if ($i % 2 != 0) {$variables['col_class'][4] = 'col-odd';}
    
    // Build the images or icons.
    if($variables['fanciblock_type'] == 'fanciblock') {
      // Use the full uploaded image for 'fullwidth' and 'none' styles.
      if($variables['settings']['icon-size'] == 'none' || $variables['settings']['icon-size'] == 'fullwidth') {
        $item['image'] = theme('image', array('uri' => $item['fanciblock_uri']));      
      }
      else {
        $item['image'] = theme('image_style', array('uri' => $item['fanciblock_uri'], 'style_name' => $variables['settings']['icon-size']));
      }
    }
    else {
      $item['image'] = theme('fanciblock_fontawesome_icon_build', array(
        'class' => $item['fontawesome_icon'], 
        'size' => $variables['icon_size'], 
        'looks' => $variables['icon_looks'], 
        'style' => $variables['icon_style'],
        'color' => 'color:'.$item['fontawesome_icon_color']
      ));
    }
    // Add boxes to 
    $variables['items'][$box] = $item;
  }

}