(function ($) {
  Drupal.behaviors.datetimeFormatter = {
    attach: function (context) {
      $.extend($.timeago.settings, Drupal.settings.datetimeFormatter.timeago);
      $('time.timeago', context).timeago();
    }
  };
})(jQuery);
