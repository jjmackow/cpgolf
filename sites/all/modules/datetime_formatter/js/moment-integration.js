(function ($, Drupal, moment) {
  var behavior = Drupal.behaviors.datetimeFormatterMoment = {};

  behavior.attach = function () {

    // Select all date ranges to rewrite, using `.once()` for AJAX compatibility.
    $('[data-rewrite-datetime]')
    .once(behavior.render)
    .on('updateTimezone', function (e, extraParameter) {
      behavior.render.call(this, e, extraParameter);
    });

    // Format any current time placeholders.
    $('[data-format-local-time]').once(function () {
      var format = $(this).data().formatLocalTime,
          result = moment().tz(moment.tz.guess()).format(format);

      $(this).text(result);
    });

    // Select all timezone select/input and bind a change listener.
    $('.js-timezone')
    .show()
    .find(':input')
    .on('change', function () {
      var value = $(this).val();

      if (!value) {
        return;
      }

      $('[data-rewrite-datetime]').trigger('updateTimezone', {timezone: value});
    });
  };

  /**
   * Render date/time from data attributes and replace the text content of the given element.
   *
   * Note: `this` must be assigned to the related DOM Element.
   *
   * @param {Anything} context
   *   If true, reset to original.
   *   This is also a placeholder for a jQuery Event argument when used as an event handler.
   * @param {Anything} extraParameter
   *   If {Object}, use timezone property
   */
  behavior.render = function (context, extraParameter) {
    var $element = $(this),
        data = $element.data(),
        typeToMethodName = {date: 'formatDateRange', time: 'formatTimeRange'},
        methodName = typeToMethodName[data.rewriteDatetime];

    // Reset to original?
    if (context === true) {
      $element.html(data.originalHtml);
    }
    // Render the date/time using this element's data attributes.
    else {
      // If we find a timezone property, use that.
      if (_.isObject(extraParameter) && extraParameter.timezone) {
        data.timezone = extraParameter.timezone;
      }
      $element.data('originalHtml', $element.html());
      $element.text(behavior[methodName](data));
    }
  };

  /**
   * Returns a fully rendered (formatted) date range.
   *
   * @param {Object} data
   *   startDate
   *   endDate
   *   formatYears
   *   formatMonths
   *   formatDays
   *   formatSingle
   */
  behavior.formatDateRange = function (data) {
    var timezone = data.timezone || moment.tz.guess(),
        startDate = moment.unix(data.startDate).tz(timezone),
        endDate = moment.unix(data.endDate).tz(timezone);

    return behavior.formatDateTimeRange(startDate, endDate, data.format);
  };

  /**
   * Returns a fully rendered (formatted) time range.
   *
   * @param {Object} data
   *   startTime
   *   endTime
   *   formatSingle
   *   formatMulti
   */
  behavior.formatTimeRange = function (data) {
    var timezone = data.timezone || moment.tz.guess(),
        startTime = moment.unix(data.startTime).tz(timezone),
        endTime = moment.unix(data.endTime).tz(timezone);

    return behavior.formatDateTimeRange(startTime, endTime, data.format);
  };

  /**
   * Formats the date/time range using Moment.
   *
   * @param startDateTime
   * @param endDateTime
   * @param format
   * @returns {*}
   */
  behavior.formatDateTimeRange = function (startDateTime, endDateTime, format) {
    var regexp = /\[(.+?)\]/g,
        match,
        result;

    // Replace curly braces with double square braces so that we preserve the end format.
    format = format.replace(/{/g, '[[').replace(/}/g, ']]');

    // Render the start date.
    result = startDateTime.format(format);

    // Find all bracketed format strings using RegExp (with global flag) and .exec() in a loop.
    while (match = regexp.exec(result)) { /* jshint ignore:line */
      // Render the end date.
      result = result.replace(match[0], endDateTime.format(match[1]));
      // Reset lastIndex of match since we shortened the string.
      regexp.lastIndex = 0;
    }

    // Return the fully rendered time.
    return result;
  };

})(jQuery, Drupal, window.moment);
