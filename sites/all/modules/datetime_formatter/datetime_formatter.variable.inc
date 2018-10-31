<?php

/**
 * Implements hook_variable_group_info().
 */
function datetime_formatter_variable_group_info() {
  $groups['datetime_formatter_timeago_js_settings'] = array(
    'title' => t('Timeago settings'),
    'description' => t('Global settings for the timeago jquery library.'),
    'access' => 'administer site configuration',
  );

  $groups['datetime_formatter_timeago_js_strings'] = array(
    'title' => t('Timeago strings'),
    'description' => t('Strings passed to the timeago jquery library.'),
    'access' => 'administer site configuration',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function datetime_formatter_variable_info($options) {
  // Settings
  $variables['datetime_formatter_timeago_refresh_millis'] = array(
    'title' => t('Refresh Timeago dates after', array(), $options),
    'type' => 'number',
    'description' => t('Timeago can update its dates without a page refresh at this interval. Leave blank or set to zero to never refresh Timeago dates.',array(), $options),
    'group' => 'datetime_formatter_timeago_js_settings',
    'required' => TRUE,
    'token' => FALSE,
    'localize' => FALSE,
    'default' => 60000,
  );

  $variables['datetime_formatter_timeago_cutoff'] = array(
    'title' => t('Do not use Timeago dates after', array(), $options),
    'type' => 'number',
    'description' => t('Leave blank or set to zero to always use Timeago dates.',array(), $options),
    'group' => 'datetime_formatter_timeago_js_settings',
    'required' => TRUE,
    'token' => FALSE,
    'localize' => FALSE,
    'default' => 3600000,
  );


  $variables['datetime_formatter_timeago_word_separator'] = array(
    'title' => t('Word separator', array(), $options),
    'type' => 'string',
    'description' => t('By default this is set to " " (a space).',array(), $options),
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => FALSE,
    'localize' => FALSE,
    'default' => ' ',
  );

  // Strings.
  $variables['datetime_formatter_timeago_prefixAgo'] = array(
    'title' => t('Prefix ago', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => NULL,
  );

  $variables['datetime_formatter_timeago_prefixFromNow'] = array(
    'title' => t('Prefix from now', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'In',
  );

  $variables['datetime_formatter_timeago_suffixAgo'] = array(
    'title' => t('Suffix ago', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'ago',
  );

  $variables['datetime_formatter_timeago_suffixFromNow'] = array(
    'title' => t('Suffix from now', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => NULL,
  );

  $variables['datetime_formatter_timeago_seconds'] = array(
    'title' => t('Seconds', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'Less than a minute',
  );

  $variables['datetime_formatter_timeago_minute'] = array(
    'title' => t('Minute', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'a minute',
  );

  $variables['datetime_formatter_timeago_minutes'] = array(
    'title' => t('Minutes', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => '%d minutes',
  );

  $variables['datetime_formatter_timeago_hour'] = array(
    'title' => t('Hour', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about an hour',
  );

  $variables['datetime_formatter_timeago_hours'] = array(
    'title' => t('Hours', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about %d hours',
  );

  $variables['datetime_formatter_timeago_day'] = array(
    'title' => t('Day', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about a day',
  );

  $variables['datetime_formatter_timeago_days'] = array(
    'title' => t('Days', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about %d days',
  );

  $variables['datetime_formatter_timeago_month'] = array(
    'title' => t('Month', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about a month',
  );

  $variables['datetime_formatter_timeago_months'] = array(
    'title' => t('Months', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about %d months',
  );

  $variables['datetime_formatter_timeago_year'] = array(
    'title' => t('Year', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about a year',
  );

  $variables['datetime_formatter_timeago_years'] = array(
    'title' => t('Years', array(), $options),
    'type' => 'string',
    'group' => 'datetime_formatter_timeago_js_strings',
    'required' => FALSE,
    'token' => TRUE,
    'localize' => TRUE,
    'default' => 'about %d years',
  );

  return $variables;
}
