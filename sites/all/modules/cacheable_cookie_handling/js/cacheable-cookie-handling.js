(function ($) {

Drupal.cacheableCookieHandling = {
  /**
   * Determines if an Ajax request to set cookies has already been started.
   */
  cookie_request_started: false,

  /**
   * Determines if an Ajax request to set cookies has already been finished.
   */
  cookie_request_finished: false,

  /**
   * Stores the new cookies that were set during the Ajax request.
   */
  cookie_request_returned_values: {},

  /**
   * Gets the value of a cookie that is set by server-side code.
   *
   * If the requested cookie is already set, its value will be passed to the
   * provided callback function. Otherwise, an Ajax request will be performed
   * to trigger all such cookies to be set (this is consolidated into a single
   * Ajax request to avoid the performance hit of multiple requests per page)
   * and the new value of the requested cookie will be passed to the provided
   * callback function.
   *
   * When calling this function in client-side code, there should be an
   * implementation of hook_cacheable_cookie_handling_set_cookies() in the
   * server-side code which actually sets the cookie that was requested here.
   *
   * Example usage:
   * @code
   * Drupal.cacheableCookieHandling.getCookie('my_cookie_name', function (cookie_value) {
   *   // This function will be called asynchronously when the cookie value is
   *   // available.
   *   alert('The value of the "my_cookie_name" cookie is: ' + cookie_value);
   * });
   * @endcode
   *
   * @param string cookie_name
   *   The name of the cookie to get.
   * @param function success_callback
   *   A function to call when the cookie's value is successfully retrieved.
   *   The cookie value and cookie name will be passed as parameters to this
   *   function.
   * @param function error_callback
   *   (Optional) A function to call when the cookie's value cannot be
   *   successfully retrieved (for example, if the server cannot be contacted,
   *   or if the server is contacted but does not set the requested cookie).
   *   The name of the requested cookie will be passed as a parameter to this
   *   function.
   */
  getCookie: function (cookie_name, success_callback, error_callback) {
    // If the cookie already exists, use its value.
    if (this._passCookieToCallback(cookie_name, success_callback)) {
      return;
    }

    // Do not perform duplicate Ajax requests. (If an Ajax request was already
    // made earlier but the cookie was never set, stop processing and treat it
    // as an error.)
    if (this.cookie_request_finished) {
      if (error_callback) {
        error_callback(cookie_name);
      }
      return;
    }

    cacheable_cookie_handling = this;

    // Request the server to set all cookies via Ajax. To prevent blocking the
    // rest of the page, this is an asynchronous request. However, if two
    // cookies are requested at once, the second must wait until the Ajax call
    // triggered by the first one is finished before returning (otherwise the
    // cookie it is looking for won't be set yet).
    if (this.cookie_request_started) {
      var interval_id = setInterval(function () {
        if (cacheable_cookie_handling.cookie_request_finished) {
          clearInterval(interval_id);
          var success = cacheable_cookie_handling._passCookieToCallback(cookie_name, success_callback);
          if (!success && error_callback) {
            error_callback(cookie_name);
          }
        }
      }, 25);
    }
    else {
      this.cookie_request_started = true;
      $.ajax({
        url: Drupal.settings.cacheable_cookie_handling.cookie_request_url,
        // This is a POST request to make sure it is not cached.
        type: 'POST',
        dataType: 'json',
        success: function (response) {
          // Save the values that were returned so they can be used by
          // subsequent requests.
          cacheable_cookie_handling.cookie_request_returned_values = response;
          cacheable_cookie_handling.cookie_request_finished = true;
          var success = cacheable_cookie_handling._passCookieToCallback(cookie_name, success_callback);
          if (!success && error_callback) {
            error_callback(cookie_name);
          }
        },
        error: function () {
          cacheable_cookie_handling.cookie_request_finished = true;
          if (error_callback) {
            error_callback(cookie_name);
          }
        }
      });
    }
  },

  /**
   * Forces an Ajax request to be triggered to update all cookies.
   *
   * In most cases Drupal.cacheableCookieHandling.getCookie() should be used
   * instead, since it has better performance, but if it is necessary to force
   * a cookie to get an updated value from the server, this can be used to do
   * that.
   *
   * @param function success_callback
   *   (Optional) A function to call when the cookies have been updated.
   */
  forceCookieUpdates: function (success_callback) {
    cacheable_cookie_handling = this;
    $.ajax({
      url: Drupal.settings.cacheable_cookie_handling.cookie_request_url,
      // This is a POST request to make sure it is not cached.
      type: 'POST',
      dataType: 'json',
      success: function (response) {
        cacheable_cookie_handling.cookie_request_returned_values = response;
        if (success_callback) {
          success_callback();
        }
      }
    });
  },

  /**
   * Passes a cookie to a callback function, if the cookie exists.
   *
   * @param string cookie_name
   *   The name of the cookie to check.
   * @param function callback
   *   A callback function to pass the cookie to. It will receive the cookie
   *   value as its first parameter and cookie name as its second parameter.
   *
   * @return bool
   *   TRUE if the cookie exists and was passed to the callback, or FALSE if
   *   not.
   */
  _passCookieToCallback: function (cookie_name, callback) {
    // If the cookie was set via an earlier Ajax request, the returned value
    // should take precedence over the cookie value itself (for some reason,
    // when a cookie is set via an Ajax call, the new value does not
    // consistently show up via $.cookie() until the next page request).
    var cookie_value = null;
    if (typeof this.cookie_request_returned_values[cookie_name] !== 'undefined') {
      cookie_value = this.cookie_request_returned_values[cookie_name];
    }
    if (cookie_value === null) {
      cookie_value = $.cookie(cookie_name);
    }
    if (cookie_value !== null) {
      callback(cookie_value, cookie_name);
      return true;
    }
    return false;
  }
};

})(jQuery);
