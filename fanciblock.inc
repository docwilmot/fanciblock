<?php

/**
 *  BlockExampleCustomClass extends Block
 *  
 *  This class allows us to create a block, set up the administrative interface 
 *  in Layout builder and set up configuration.
 */
class FanciblockClass extends Block {
  /**
   *  Sets title text on draggable block panel in Layout builder.
   *  
   *  Defaults to the 'title' attribute set in hook_block_info
   */
  function getAdminTitle() {
    return t('Fanciblock');
  }

  /**
   *  Sets description text on draggable block in Layout builder.
   *  
   *  Defaults to the 'description' attribute set in hook_block_info
   */
  function getAdminPreview() {
    return $this->delta == 'fanciicon' ? t('Fanciicon Block') : t('Fanciimage Block');
  }

  /**
   *  Sets block content on block view.
   */
  function getContent() {
    $data['content'] =  array(
      '#markup' => fanciblock_build_display($this->delta, $this->settings),
    );
    return render($data);
  }

  /**
   *  Builds the block's configuration form.
   */
  function form(&$form, &$form_state) {
    module_load_include('inc', 'fanciblock', 'fanciblock.admin');
    $settings = $form_state['block']->settings;
    $settings = _flatten_fanciblock_settings ($settings['fanciblock_settings']);
    $delta = $form_state['block']->delta;
    $form += fanciblock_edit_box('edit', $delta, $settings);
  }
  
  /**
   * Submit handler to save the form settings.
   *
   * Settings are saved to the active layout configuration file.
   */
  function formSubmit($form, &$form_state) {
    $config = config('fanciblock.settings');
    // Set up defaults.
    $default_fid = $config->get('default_fid');
    $default_fanciblock_uri = $config->get('default_fanciblock_uri');
    $default_box = array(
      'fanciblock_name' => $form_state['values']['fanciblock_name'], 
      'boxid' => '', 
      'box_title' => '', 
      'weight' => 0, 
      'box_content' => 'This is Fancibox content!',
      'icon_link' => '',
    );
    $delta = $form_state['block']->delta;
    if ($delta == 'fanciblock') {
      $default_box += array(
        'fanciblock_fid' => $default_fid, 
        'fanciblock_uri' => $default_fanciblock_uri, 
      );
    }
    elseif($delta == 'fanciicon') {
      $default_box += array(
        'fontawesome_icon' => 'fa-tablet',
        'fontawesome_icon_color' => '#333333',
      );
    }

    form_state_values_clean($form_state);

    // Check and see if this block's settings already exist. Theyre saved in
    // config.
    $settings = $this->settings['fanciblock_settings'];
    $box_settings = array();
    if($settings && $settings['box_settings']) {
      $box_settings = $settings['box_settings'];
    }
    
    $boxes = array();
    $this->settings['fanciblock_settings'] = $form_state['values'];
    $this->settings['fanciblock_settings']['box_settings'] = $box_settings;

    // If we dont find the settings, this is a new block being built, so
    // build and save a default box for each grid.
    if(!empty($settings['box_settings'])) {
      $boxes = _fanciblock_add_default_boxes($form_state['values']['grid']['grid'], $default_box, $delta);
      $this->settings['fanciblock_settings']['box_settings'] = $boxes;
    }
    // We've got boxes already, let's see if we're changing how many.
    // We don't do any manipulation of boxes here, there is a separate form. See
    // fanciblock_box_overview_form().
    else {
      // If we are adding boxes, Grid number will be more than the number of 
      // settings.
      $box_count = count($settings['box_settings']);
      if($form_state['values']['grid']['grid'] > $box_count) {
        $adding = $form_state['values']['grid']['grid'] - count($settings);
        $boxes = _fanciblock_add_default_boxes($adding, $default_box, $delta);      
        $this->settings['fanciblock_settings']['box_settings'] += $boxes;
      }
      // Theres no safe way to decrease the number of boxes, so we dont allow that.
      elseif($form_state['values']['grid']['grid'] < $box_count) {
        form_set_error('', 'Cannot decrease number of boxes, please delete box instead');
      }
    }
  }
}
/**
 * @} End of "defgroup block_example".
 */
