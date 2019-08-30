<?php

/**
 * Page callback: Athenapdf settings
 *
 * @see athenapdf_menu()
 */
function athenapdf_admin_form($form, &$form_state) {
  global $base_url, $base_path;

  $description = <<<EOL
  This section contains the configuration for the AthenaPDF Waver, which is the microservice used to handle requests via http. <br/>
  For more info refer to the <a href="@doc_url" target="_blank">AthenaPDF documentation</a> on GitHub.
EOL;
  $form['athenapdf_weaver'] = [
    '#type'        => 'fieldset',
    '#title'       => 'AthenaPDF Waver',
    '#description' => t(
      $description,
      ['@doc_url' => 'https://github.com/arachnys/athenapdf#microservice']
    ),
    '#tree'        => FALSE,
  ];

  $form['athenapdf_weaver']['athenapdf_weaver_domain'] = [
    '#type'          => 'textfield',
    '#title'         => t('Domain'),
    '#description'   => t(
      'The AthenaPDF Weaver domain (Must be reachable from Drupal).'
    ),
    '#default_value' => variable_get('athenapdf_weaver_domain'),
    '#required'      => TRUE,
    '#attributes'    => ['placeholder' => 'https://example.com'],
  ];

  $form['athenapdf_weaver']['athenapdf_weaver_key'] = [
    '#type'          => 'textfield',
    '#title'         => t('Auth Key'),
    '#description'   => t(
      'The authorization key for the weaver service. (AthenaPDF default: %default_key)',
      ['%default_key' => 'arachnys-weaver']
    ),
    '#default_value' => variable_get('athenapdf_weaver_key'),
    '#required'      => TRUE,
  ];


  $form['athenapdf_advanced'] = [
    '#type'        => 'fieldset',
    '#title'       => 'Advanced Config',
    '#tree'        => FALSE,
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  ];

  $form['athenapdf_advanced']['athenapdf_storage_type'] = [
    '#type'          => 'select',
    '#title'         => t('PDF cache destination'),
    '#description'   => t(
      'Destination where the generated pdfs will be stored for caching purposes.'
    ),
    '#default_value' => variable_get('athenapdf_storage_type', 'public'),
    '#options'       => _athenapdf_get_stream_wrappers_as_options(),
    '#required'      => TRUE,
  ];

  $form['athenapdf_advanced']['athenapdf_front_domain'] = [
    '#type'          => 'textfield',
    '#title'         => t('Front Base URL'),
    '#description'   => t(
      'This address will be used by athenapdf daemon to generate the PDF, leave empty to use the default site url.'
    ),
    '#default_value' => variable_get('athenapdf_front_domain'),
  ];

  return system_settings_form($form);
}

/**
 * Returns the available stream wrappers as form options.
 */
function _athenapdf_get_stream_wrappers_as_options() {
  $stream_wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_LOCAL_NORMAL);

  $options = [];
  foreach ($stream_wrappers as $machine_name => $stream_wrapper) {
    $options[$machine_name] = $stream_wrapper['name'];
  }

  return $options;
}
