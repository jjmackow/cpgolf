<?php

/**
 * @file
 * Hooks provided by the Cacheable Cookie Handling module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Set and return cookies managed by the Cacheable Cookie Handling module.
 *
 * This hook is used to set any cookies your module provides that are managed
 * by the Cacheable Cookie Handling module. The hook will be automatically
 * invoked whenever client-side code issues an Ajax request for a cookie that
 * it needs but that isn't currently available.
 *
 * By design, this hook is invoked via a menu callback that all users have
 * access to. Therefore, if the cookies your module sets should only ever be
 * made available to certain users, it is your responsibility to check those
 * conditions within this hook before setting and returning the cookies.
 *
 * Any cookies set in this hook should only be accessed in client-side code
 * using Drupal.cacheableCookieHandling.getCookie() (not directly via
 * jQuery.cookie()).
 *
 * Do not use this hook for cookies that must be available to client-side code
 * early in the page request (for example, cookies that other JavaScript on the
 * site might rely on, not just your own JavaScript) since the Ajax requests to
 * set the cookies are done asynchronously and therefore the cookie value might
 * only be available for use once that Ajax request completes. See the
 * documentation in Drupal.cacheableCookieHandling.getCookie() for additional
 * details.
 *
 * If your cookie has a short lifetime, you can still use this hook, but be
 * aware that it may trigger frequent Ajax requests to the server to set the
 * cookie (every time it expires). This hook is most useful for cookies that
 * have long lifetimes, such as session cookies.
 *
 * @return
 *   An array of cookies that were set, where the array keys are cookie names
 *   and array values are cookie values. The hook implementation must both set
 *   the relevant cookies *and* return them in this array in order to work
 *   correctly.
 */
function hook_cacheable_cookie_handling_set_cookies() {
  if (my_module_current_user_should_get_the_cookie()) {
    $cookie_value = my_module_get_cookie_for_current_user();
    // Note that the $httponly parameter to setcookie() should never be used
    // here; doing so would make the cookie unavailable to client-side code and
    // could have other unexpected results.
    setcookie('my_module_session_cookie', $cookie_value);
    return array('my_module_session_cookie' => $cookie_value);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
