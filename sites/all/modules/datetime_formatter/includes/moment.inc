<?php
/**
 * @file
 * Moment.js integration.
 *
 * Adapted from the Drupal moment contrib module.
 */

/**
 * Implements hook_libraries_info().
 */
function datetime_formatter_libraries_info() {
  $libraries = array();

  if (($library_path = libraries_get_path('moment'))) {
    $known_files = array(
      'moment.js',
      'min/moment.min.js',
      'min/moment-with-locales.js',
      'min/moment-with-locales.min.js',
    );
    $found_files = array_filter($known_files, function ($file) use ($library_path) {
      return file_exists("$library_path/$file");
    });

    $libraries['moment'] = array(
      'library path' => $library_path,
      'name' => 'Moment.js',
      'vendor url' => 'http://momentjs.com',
      'download url' => 'https://github.com/moment/moment/releases',
      'version arguments' => array(
        'file' => $found_files ? $found_files[0] : NULL,
        'pattern' => '/version : ([\d\.]+)/',
        'lines' => 5,
      ),
      'files' => array(
        'js' => array(
          'min/moment.min.js',
        ),
      ),
      'variants' => array(
        'none' => array(
          'files' => array(
            'js' => array(
              'moment.js',
            ),
          ),
        ),
        'with-locales.none' => array(
          'callbacks' => array(
            'pre-load' => array(),
          ),
          'files' => array(
            'js' => array(
              'min/moment-with-locales.js',
            ),
          ),
        ),
        'with-locales.min' => array(
          'callbacks' => array(
            'pre-load' => array(),
          ),
          'files' => array(
            'js' => array(
              'min/moment-with-locales.min.js',
            ),
          ),
        ),
      ),
      'integration files' => array(
        'datetime_formatter' => array(
          'js' => array(
            'js/moment-integration.js' => array(
              'weight' => 90,
            ),
          ),
        ),
      ),
    );

    if (($library_path = libraries_get_path('moment-timezone'))) {
      $known_files = array(
        'moment-timezone.js',
        'builds/moment-timezone.min.js',
        'builds/moment-timezone-with-data.js',
        'builds/moment-timezone-with-data.min.js',
        'builds/moment-timezone-with-data-2010-2020.js',
        'builds/moment-timezone-with-data-2010-2020.min.js',
      );
      $found_files = array_filter($known_files, function ($file) use ($library_path) {
        return file_exists("$library_path/$file");
      });

      $libraries['moment-timezone'] = array(
        'library path' => $library_path,
        'name' => 'Moment Timezone',
        'vendor url' => 'http://momentjs.com/timezone',
        'download url' => 'https://github.com/moment/moment-timezone/releases',
        'version arguments' => array(
          'file' => $found_files ? $found_files[0] : NULL,
          'pattern' => '/version : ([\d\.]+)/',
          'lines' => 5,
        ),
        'files' => array(
          'js' => array(
            'builds/moment-timezone-with-data.min.js',
          ),
        ),
        'dependencies' => array(
          'moment',
        ),
        'variants' => array(
          'without-data.none' => array(
            'files' => array(
              'js' => array(
                'moment-timezone.js',
              ),
            ),
          ),
          'without-data.min' => array(
            'files' => array(
              'js' => array(
                'builds/moment-timezone.min.js',
              ),
            ),
          ),
          'with-data.none' => array(
            'files' => array(
              'js' => array(
                'builds/moment-timezone-with-data.js',
              ),
            ),
          ),
          'with-data.min' => array(
            'files' => array(
              'js' => array(
                'builds/moment-timezone-with-data.min.js',
              ),
            ),
          ),
          'with-data-2010-2020.none' => array(
            'files' => array(
              'js' => array(
                'builds/moment-timezone-with-data-2010-2020.js',
              ),
            ),
          ),
          'with-data-2010-2020.min' => array(
            'files' => array(
              'js' => array(
                'builds/moment-timezone-with-data-2010-2020.min.js',
              ),
            ),
          ),
        ),
      );
    }
  }

  return $libraries;
}

/**
 * Library info "version callback".
 *
 * @param array $info
 *   Library info.
 *
 * @return string|false
 *   Version number.
 */
function datetime_formatter_moment_get_library_version(array $info) {
  return datetime_formatter_moment_get_package_version($info['library path']);
}

/**
 * Get the version number from package JSON.
 *
 * @param string $library_path
 *   Directory.
 *
 * @return string|false
 *   Version number.
 */
function datetime_formatter_moment_get_package_version($library_path) {
  static $versions = array();

  if (!$library_path) {
    return FALSE;
  }

  if (!isset($version[$library_path])) {
    $versions[$library_path] = FALSE;
    if (is_readable("$library_path/package.json")) {
      $package = drupal_json_decode(file_get_contents("$library_path/package.json"));
      if ($package && !empty($package['version'])) {
        $versions[$library_path] = $package['version'];
      }
    }
  }

  return $versions[$library_path];
}

/**
 * The format replacement patterns for the Moment.js library.
 *
 * @see http://php.net/date
 * @see http://momentjs.com/docs/#/displaying/format
 *
 * @return array
 *   Key is the PHP date format character, the value is the corresponding
 *   Moment.js format string.
 */
function datetime_formatter_get_date_format_replacements() {
  // PHP => Moment.js.
  // PHP does not support the following formats:
  // @todo Complete this list.
  // Seconds              0 1 ... 58 59.
  // '?' => 's'
  // Day of Week        Su Mo ... Fr Sa.
  // '?' => 'dd'
  return array(
    // Hour             01 02 ... 11 12.
    'h' => 'hh',
    // Hour             00 01 ... 22 23.
    'H' => 'HH',
    // Hour              1  2 ... 11 12.
    'g' => 'h',
    // Hour              0  1 ... 22 23.
    'G' => 'H',

    // Minute           00 01 ... 58 59.
    'i' => 'mm',

    // Seconds.         00 01 ... 58 59.
    's' => 'ss',

    // Year         1970 1971 ... 2029 2030
    'Y' => 'YYYY',
    // Year           70   71 ...   29   30
    'y' => 'YY',

    // Month             1  2 ... 11 12.
    'n' => 'M',
    // Month            01 02 ... 11 12.
    'm' => 'MM',
    // Month          Jan Feb ... Nov Dec.
    'M' => 'MMM',
    // Month January February ... November December.
    'F' => 'MMMM',

    // Day of Year    001 002 ... 364 365.
    'z' => 'DDDD',

    // Day of Month      1  2 ... 30 31.
    'j' => 'D',
    // Day of Month     01 02 ... 30 31.
    'd' => 'DD',
    // Day of Month   1st 2nd ... 30th 31st
    'jS' => 'Do',

    // Day of Week        0 1 ... 5 6.
    'w' => 'd',
    // Day of Week (ISO)  1 2 ... 6 7.
    'N' => 'E',
    // Day of Week    Sun Mon ... Fri Sat.
    'D' => 'ddd',
    // Day of W Sunday Monday ... Friday Saturday.
    'l' => 'dddd',

    // Week of Year       1 2 ... 52 53
    'W' => 'w',

    // Timezone -07:00 -06:00 ... +06:00 +07:00.
    'P' => 'Z',
    // Timezone   -0700 -0600 ... +0600 +0700.
    'O' => 'ZZ',

    // Shortcut to: Y-m-d\TH:i:sP
    // Example:     2004-02-12T15:19:21+00:00
    'c' => 'YYYY-MM-DDTHH:mm:ssZ',
    // Shortcut to: D, d M Y H:i:s O
    // Example:     Thu, 21 Dec 2000 16:01:07 +0200
    'r' => 'ddd, DD MMM YYYY HH:mm:ss ZZ',

    // Unix Timestamp.
    'U' => 'X',

    // Time zone.
    'T' => 'z',
  );
}

/**
 * Convert a date format to Moment.js compatible date format.
 *
 * @param string $date_format
 *   PHP date format that is suitable for date().
 *
 * @return string
 *   Date format which is suitable for Moment.js.
 */
function datetime_formatter_php_to_moment_format($date_format) {
  return strtr($date_format, datetime_formatter_get_date_format_replacements());
}

