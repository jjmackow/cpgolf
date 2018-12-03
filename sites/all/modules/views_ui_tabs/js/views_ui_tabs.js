(function ($) {

  Drupal.behaviors.ViewsUITabs = {
    attach: function(context) {
      var ids = Drupal.settings.views_ui_tabs;
      $.each(ids, function(id, o) {
        $('#' + id, context).once('views-ui-tabs', function() {
          var $t = $(this);
          $t.tabs(o);
          if (o.sortable) {
            $t.find( ".ui-tabs-nav" ).sortable({ axis: "x" });
          }
          if (o.tabs_create_url) {
            $t.find( ".ui-tabs-nav" ).append('<li class="views-ui-tabs-create-tab ui-state-default ui-corner-top"><a href="' + o.tabs_create_url + '">' + o.tabs_create_text + '</a></li>');
          }
        });
      });
    }
  };

})(jQuery);
