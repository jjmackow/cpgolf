CACHEABLE COOKIE HANDLING
-------------------------

This module provides a framework for setting cookies that works well with page caching.  It does not do anything on its own, but rather provides an API that other modules can use.

If your module needs to set cookies that depend on server-side information but that need to be accessed client-side by JavaScript, it can be difficult to do that in a way that is compatible with page caching. For example, if you set the cookie server-side when building the page, or if you put the relevant information into the HTML (via Drupal.settings) when building the page and then let the client-side code use that to set the cookie, the page cannot be properly cached since user-specific information was involved in building it. This can be a particular problem on sites that cache pages for authenticated users.

This module provides a simple, scalable solution to the problem:
- It provides a hook, hook_cacheable_cookie_handling_set_cookies(), which your module can implement to set any cookies that fit the above criteria. This is the only place you need to set the cookie.
- It provides a JavaScript library with an API function, Drupal.cacheableCookieHandling.getCookie(), which your module's JavaScript can call whenever you need to access the cookie.

Behind the scenes, the Cacheable Cookie Handling module will send a single Ajax request to ask the server to set all relevant cookies, only when JavaScript asks for a cookie that isn't already available. The Ajax request is uncached, but because everything is consolidated into a single request it should rarely need to be invoked (it will only need to be invoked again if some of the cookies expire or are deleted). And since regular Drupal page requests do not set the cookie, they can be safely cached like normal.

For additional information, see the project page: https://www.drupal.org/project/cacheable_cookie_handling

ADVANCED USAGE
--------------

For even better performance, you can consider forcing the cookies to be set on particular server-side page requests that are already uncached. Typically a login request is a good time to do that. This can result in the Ajax request not needing to be triggered at all under normal circumstances. For example, in site-specific code:

<?php
/**
 * Implements hook_user_login().
 */
function MYMODULE_user_login() {
  // If the user is logging in via a POST request, the page will already be
  // uncacheable, so we might as well set all the cookies that the user will
  // need here also.
  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    cacheable_cookie_handling_set_cookies();
  }
}
?>

CREDITS
-------

Initial development of this project was supported by Tag1 Consulting (https://tag1consulting.com).
