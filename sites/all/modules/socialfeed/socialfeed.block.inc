<?php

/**
 * @file
 * File include for Social Feed module.
 */

/**
 * Return HTML with posts.
 *
 * @param string $provider
 *   Accepts string as value.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialfeed_posts_render($provider) {
  $fn = 'socialfeed_' . $provider . '_posts';
  $message_feed = call_user_func($fn);
  return theme('socialfeed_' . $provider, array($provider => $message_feed));
}

/**
 * Rendering values from the Facebook feed.
 *
 * @param int $i
 *   Accepts int as value.
 * @param string $facebook_entry
 *   Accepts string as value.
 * @param bool $display_all_posts
 *   Accepts bool as value.
 * @param bool $display_time
 *   Accepts bool as value.
 * @param bool $display_pic
 *   Accepts bool as value.
 * @param bool $display_video
 *   Accepts bool as value.
 * @param string $teaser_text
 *   Accepts string as value.
 * @param string $facebook_hash_tag
 *   Accepts string as value.
 *
 * @return mixed
 *   Returns mixed.
 */
function socialfeed_facebook_data($i, $facebook_entry, $display_all_posts, $display_time, $display_pic, $display_video, $teaser_text, $facebook_hash_tag) {
  $trim_length = variable_get('socialfeed_facebook_trim_length');
  if (array_key_exists('message', $facebook_entry)) {
    if (isset($facebook_entry['full_picture']) && $display_all_posts == 0) {
      if ($display_pic == 1) {
        $message_feed['full_picture'] = $facebook_entry['full_picture'];
      }
    }
    if (isset($facebook_entry['source']) && $display_all_posts == 0) {
      if ($display_video == 1) {
        $message_feed['video'] = $facebook_entry['source'];
      }
    }
    if (isset($facebook_entry['message'])) {
      if (isset($trim_length) && !empty($trim_length)) {
        $trimmed_message = substr($facebook_entry['message'], 0, $trim_length);
        $message_feed['message'] = $trimmed_message;
      }
      else {
        $message_feed['message'] = substr($facebook_entry['message'], 0, 200);
      }
    }
    if (isset($teaser_text) && !empty($teaser_text) && isset($facebook_entry['permalink_url'])) {
      $message_feed['full_feed_link'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $facebook_entry['permalink_url'], array('attributes' => array('target' => '_blank')));
    }
    else {
      $message_feed['full_feed_link'] = t('@teaser_text', array('@teaser_text' => $teaser_text));
    }

    if ($facebook_hash_tag == 1) {
      $message_feed['message'] = preg_replace_callback('/#(\\w+)/', function ($hash) {
        return l($hash[0], 'https:facebook.com/hashtag/' . $hash[1], array('attributes' => array('target' => '_blank')));
      }, $message_feed['message']);
    }
    if ($display_time == 1) {
      $formatted_date = new DateTime($facebook_entry['created_time']);
      $message_feed['created_stamp'] = $formatted_date->format(variable_get('socialfeed_facebook_time_format'));
    }
  }
  else {
    if (isset($facebook_entry['full_picture']) && $display_all_posts == 0) {
      if ($display_pic == 1) {
        $message_feed['full_picture'] = $facebook_entry['full_picture'];
      }
    }
    if (isset($facebook_entry['source']) && $display_all_posts == 0) {
      if ($display_video == 1) {
        $message_feed['video'] = $facebook_entry['source'];
      }
    }

    if (isset($facebook_entry['message']) && !empty($facebook_entry['message'])) {
      $message_feed['message'] = substr($facebook_entry['message'], 0, variable_get('socialfeed_facebook_trim_length'));
    }
    if (isset($teaser_text) && !empty($teaser_text) && isset($facebook_entry['permalink_url'])) {
      $message_feed['full_feed_link'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $facebook_entry['permalink_url'], array('attributes' => array('target' => '_blank')));
    }
    else {
      $message_feed['full_feed_link'] = t('@teaser_text', array('@teaser_text' => $teaser_text));
    }
  }

  return $message_feed;
}

/**
 * Returns HTML with feeds in required format.
 *
 * @param string $page_name
 *   Accepts string as value.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialfeed_facebook_feeds($page_name = '') {
  global $user;
  $selected_type_value = $config = $message_feed = array();
  $display_time = $display_pic = $display_video = $display_all_posts = $teaser_text = $facebook_hash_tag = $selected_type = $selected_type_key = $fb_type = $selected_type_value = $url = $request = '';
  $config['app_id'] = variable_get('socialfeed_facebook_app_id');
  $config['secret'] = variable_get('socialfeed_facebook_secret_key');
  $config['page_name'] = variable_get('socialfeed_facebook_page_name');

  if (isset($config['app_id']) && !empty($config['app_id']) && isset($config['secret']) && !empty($config['secret'])) {
    $url = "https://graph.facebook.com/" . $config['page_name'] . "/feed?access_token=" . $config['app_id'] . "|" . $config['secret'] . '&fields=id,type,created_time,message,full_picture,permalink_url,source';
    $request = drupal_http_request($url);

    if ($request->status_message && $request->status_message == "OK") {
      $display_time = variable_get('socialfeed_facebook_time_stamp');
      $display_pic = variable_get('socialfeed_facebook_display_pic');
      $display_video = variable_get('socialfeed_facebook_display_video');
      $display_all_posts = variable_get('socialfeed_facebook_all_types');
      $teaser_text = variable_get('socialfeed_facebook_teaser_text');
      $facebook_hash_tag = variable_get('socialfeed_facebook_hashtag');
      $selected_type = variable_get('socialfeed_facebook_post_type');
      // @TODO: Check why this is used?
      $selected_type_key = $selected_type;
      $fb_type = array('link', 'photo', 'status', 'video');
      if (isset($selected_type_key)) {
        $selected_type_value = $fb_type[$selected_type_key];
      }
      $i = 0;
      $post_type_filter = $next_post_type_filter = array();
      $json_response = drupal_json_decode($request->data);

      if (isset($json_response['data']) && !empty($json_response['data'])) {
        foreach ($json_response['data'] as $facebook_value) {
          // If specific post type is selected.
          if ($display_all_posts == 0 && isset($facebook_value['type']) && $facebook_value['type'] == $selected_type_value) {
            $post_type_filter[$facebook_value['type']][] = $facebook_value;
          }
          // If all post type selected.
          elseif ($display_all_posts == 1) {
            $message_feed[] = socialfeed_facebook_data($i, $facebook_value, $display_all_posts, $display_time, $display_pic, $display_video, $teaser_text, $facebook_hash_tag);
            $i++;
            if ($i == variable_get('socialfeed_facebook_no_feeds')) {
              break;
            }
          }
        }
      }
      // Showcase of array of specific post type feeds.
      if (array_key_exists($selected_type_value, $post_type_filter)) {
        foreach ($post_type_filter[$selected_type_value] as $facebook_entry) {
          $message_feed[] = socialfeed_facebook_data($i, $facebook_entry, $display_all_posts, $display_time, $display_pic, $display_video, $teaser_text, $facebook_hash_tag);
          $i++;
          if ($i == variable_get('socialfeed_facebook_no_feeds')) {
            break;
          }
        }
        $existing_feeds = count($post_type_filter[$selected_type_value]);
        $remaining = variable_get('socialfeed_facebook_no_feeds') - $existing_feeds;
        if ($existing_feeds < variable_get('socialfeed_facebook_no_feeds')) {
          $i = 0;
          $pager_next = $json_response['paging']['next'];
          $next_page_json = file_get_contents($pager_next);
          $next_page = json_decode($next_page_json);
          foreach ($next_page->data as $next_facebook_value) {
            if ($display_all_posts == 0 && $next_facebook_value->type == $selected_type_value) {
              $next_post_type_filter[$next_facebook_value->type][] = $next_facebook_value;
            }
          }
          if (array_key_exists($selected_type_value, $next_post_type_filter)) {
            foreach ($next_post_type_filter[$selected_type_value] as $facebook_entry) {
              $message_feed[] = socialfeed_facebook_data($i, $facebook_entry, $display_all_posts, $display_time, $display_pic, $display_video, $teaser_text, $facebook_hash_tag);
              $i++;
              if ($i == $remaining) {
                break;
              }
            }
          }
        }
      }
      else {
        if (empty($message_feed)) {
          drupal_set_message(t('No related posts'), 'error');
        }
      }

      return $message_feed;
    }
    else {
      drupal_set_message(t('The access_token provided is invalid.'), 'error');
    }
  }
  else {
    if (in_array('administrator', array_values($user->roles))) {
      drupal_set_message(t('Please provide your Facebook credentials <a href="@facebook">here</a>.', array('@facebook' => url('admin/config/services/socialfeed'))), 'warning');
    }
    else {
      drupal_set_message(t('Please login to provide Facebook App values <a href="@facebook">here</a>.', array('@facebook' => url('user'))), 'error');
    }
  }
}

/**
 * Uses socialfeed_facebook_posts() for fetching Facebook feeds.
 */
function socialfeed_facebook_posts() {
  $facebook_latest = socialfeed_facebook_feeds(variable_get('socialfeed_facebook_page_name'));

  return $facebook_latest;
}

/**
 * Uses socialfeed_twitter_posts() for fetching Twitter tweets.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialfeed_twitter_posts() {
  $twitter_tweets = array();
  $tweets_count = variable_get('socialfeed_twitter_tweets_count');
  $twitter_username = variable_get('socialfeed_twitter_username');
  $display_time = variable_get('socialfeed_twitter_time_stamp');
  $display_date_twitter_style = variable_get('socialfeed_twitter_time_ago');
  $twitter_hash_tag = variable_get('socialfeed_twitter_hashtag');
  $teaser_text = variable_get('socialfeed_twitter_teaser_text');
  $twitter_consumer_key = variable_get('socialfeed_twitter_consumer_key');
  $twitter_consumer_secrete = variable_get('socialfeed_twitter_consumer_secret');

  // Authentication Parameters.
  $api_key = urlencode($twitter_consumer_key);
  $api_secret = urlencode($twitter_consumer_secrete);
  $auth_url = 'https://api.twitter.com/oauth2/token';

  if (!empty($api_key) && !empty($api_secret)) {
    // What we want?
    $data_username = $twitter_username;
    $data_count = $tweets_count;
    $data_url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';

    // Get API Access Token.
    $api_credentials = base64_encode($api_key . ':' . $api_secret);
    $auth_headers = 'Authorization: Basic ' . $api_credentials . "\r\n" . 'Content-Type: application/x-www-form-urlencoded;charset=UTF-8' . "\r\n";
    $auth_context = stream_context_create(array(
      'http' => array(
        'header' => $auth_headers,
        'method' => 'POST',
        'content' => http_build_query(array(
          'grant_type' => 'client_credentials',
        )),
      ),
    ));
    $auth_response = json_decode(file_get_contents($auth_url, 0, $auth_context), TRUE);
    $auth_token = $auth_response['access_token'];

    // Get Tweets.
    $data_context = stream_context_create(array('http' => array('header' => 'Authorization: Bearer ' . $auth_token . "\r\n")));
    $twitter_values = json_decode(file_get_contents($data_url . '?count=' . $data_count . '&tweet_mode=extended&screen_name=' . urlencode($data_username), 0, $data_context), TRUE);

    // Results - Do what you want!
    foreach ($twitter_values as $key => $twitter_value) {
      $twitter_tweets[$key]['username'] = $twitter_value['user']['screen_name'];
      $twitter_tweets[$key]['full_username'] = 'http://twitter.com/' . $twitter_value['user']['screen_name'];
      preg_match_all('#\bhttps?://[^\s()<>]+(?:\([\w\d]+\)|([^[:punct:]\s]|/))#', $twitter_value['full_text'], $extra_links);
      foreach ($extra_links[0] as $extra_link) {
        $twitter_tweets[$key]['extra_links'][] = $extra_link;
      }
      if (isset($twitter_value['full_text'])) {
        if (!empty($extra_link)) {
          $twitter_tweets[$key]['tweet'] = substr(rtrim($twitter_value['full_text'], $extra_link), 0, variable_get('socialfeed_twitter_trim_length'));
        }
        else {
          $twitter_tweets[$key]['tweet'] = substr(rtrim($twitter_value['full_text']), 0, variable_get('socialfeed_twitter_trim_length'));
        }
      }
      if (isset($teaser_text) && !empty($teaser_text)) {
        if (array_key_exists('media', $twitter_value['entities'])) {
          $twitter_tweets[$key]['tweet_url'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $twitter_value['entities']['media'][0]['url'], array('attributes' => array('target' => '_blank')));
        }
      }
      if (!empty($twitter_value['entities']['media'])) {
        $twitter_tweets[$key]['media'] = $twitter_value['entities']['media'];
      }
      if ($display_time == 1) {
        $formatted_twitter_date = new DateTime($twitter_value['created_at']);
        $reflection_object = new ReflectionObject($formatted_twitter_date->setTimezone(new DateTimeZone(drupal_get_user_timezone())));
        $get_property = $reflection_object->getProperty('date');
        $date = $get_property->getValue($formatted_twitter_date);
        if ($display_date_twitter_style == 1) {
          $twitter_tweets[$key]['twitter_date'] = socialfeed_time_elapsed_string($date);
        }
        else {
          $twitter_tweets[$key]['twitter_date'] = $formatted_twitter_date->format(variable_get('socialfeed_twitter_time_format'));
        }
      }
      if ($twitter_hash_tag == 1) {
        $twitter_tweets[$key]['tweet'] = preg_replace_callback('/#(\\w+)|@(\\w+)/', function ($hash) {
          if ($hash[0][0] == '#') {
            return l($hash[0], '//twitter.com/hashtag/' . $hash[1], array('attributes' => array('target' => '_blank')));
          }
          if ($hash[0][0] == '@') {
            return l($hash[0], '//twitter.com/' . $hash[2], array('attributes' => array('target' => '_blank')));
          }
        }, $twitter_tweets[$key]['tweet']);
      }
    }
    return $twitter_tweets;
  }
  drupal_set_message(t('Please provide your Twitter credentials <a href="@twitter">here</a>.', array('@twitter' => url('admin/config/services/socialfeed/twitter'))), 'warning');

  return array();
}

/**
 * Displays date in Twitter format.
 *
 * @param string $datetime
 *   Accepts string as value.
 * @param bool $full
 *   Accepts bool as value.
 *
 * @return string
 *   Returns string.
 */
function socialfeed_time_elapsed_string($datetime, $full = FALSE) {
  $now = new DateTime();
  $ago = new DateTime($datetime);
  $diff = $now->diff($ago);

  $diff->w = floor($diff->d / 7);
  $diff->d -= $diff->w * 7;

  $string = array(
    'y' => 'year',
    'm' => 'month',
    'w' => 'week',
    'd' => 'day',
    'h' => 'hour',
    'i' => 'minute',
    's' => 'second',
  );
  foreach ($string as $k => &$v) {
    if ($diff->$k) {
      $v = $diff->$k . ' ' . t($v . ($diff->$k > 1 ? 's' : ''));
    }
    else {
      unset($string[$k]);
    }
  }

  if (!$full) {
    $string = array_slice($string, 0, 1);
  }
  return $string ? t("@time ago", array('@time' => implode(', ', $string))) : t('just now');
}

/**
 * Uses socialfeed_instagram_posts() for fetching instagram pictures.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialfeed_instagram_posts() {
  $i = 0;
  $images = $pic = array();
  $instagram_post_link = variable_get('socialfeed_instagram_post_link');
  $resolution = variable_get('socialfeed_instagram_picture_resolution');
  $access_token = variable_get('socialfeed_instagram_access_token');
  $url = "https://api.instagram.com/v1/users/self/media/recent?access_token=" . variable_get('socialfeed_instagram_access_token') . '&count=' . variable_get('socialfeed_instagram_picture_count');
  $request = drupal_http_request($url);
  $status = strtoupper($request->status_message);

  if ($status == 'OK') {
    if (isset($access_token) && !empty($access_token)) {
      $json_response = drupal_json_decode($request->data);
      foreach ($json_response['data'] as $key => $response_data) {
        if ($instagram_post_link == 1) {
          $images[$key]['post_url'] = $response_data['link'];
        }
        $pic[] = $response_data['images'];
        $i++;
        if ($i == variable_get('socialfeed_instagram_picture_count')) {
          break;
        }
      }

      foreach ($pic as $key => $image) {
        $images[$key]['image_url'] = $image[$resolution]['url'];
      }
      return $images;
    }
    else {
      drupal_set_message(t('The access_token provided is invalid.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Please provide your Instagram credentials <a href="@instagram">here</a>.', array('@instagram' => url('admin/config/services/socialfeed/instagram'))), 'warning');
  }
}
