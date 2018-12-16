/**
 * @file
 * Paragraphs Previewer handling.
 */

(function ($, Drupal, window) {
  'use strict';

  Drupal.behaviors.paragraphsPreviewer = {
    attach: function (context, settings) {
      $('.paragraphs-previewer-button', context).once('paragraphs-previewer').click(function(event) {
        event.preventDefault();

        var $t = $(this);
        var url = $t.attr('href');
        if (url && typeof url !== 'undefined') {
          // Padding of 1%.
          var dialogPaddingPercent = 0.01;

          var dialogOptions = {
            dialogClass: 'paragraphs-previewer-ui-dialog',
            minHeight: 100,
            minWidth: 320,
            width: $(window).width() * (1 - 2 * dialogPaddingPercent),
            autoOpen: true,
            modal: true,
            draggable: true,
            autoResize: false,
            resizable: true,
            closeOnEscape: true,
            title: $t.data('dialogTitle') || null,
            closeText: '',
            open: function(event, ui) {
              $('body').addClass('paragraphs-previewer-dialog-active');
              var tDialog = this;
              $('.paragraphs-previewer-iframe', this).load(function() {
                $(tDialog).addClass('paragraphs-previewer-wrapper-loaded');
              });
            },
            close: function(event, ui) {
              $('body').removeClass('paragraphs-previewer-dialog-active');
            }
          };

          // Set height and adjust for admin toolbar.
          dialogOptions.height = $(window).height();
          var $toolbar = $('#toolbar');
          if ($toolbar.length > 0) {
            var toolbarHeight = $toolbar.outerHeight();
            if (toolbarHeight > 0) {
              dialogOptions.height -= toolbarHeight;

              dialogOptions.position = {
                my: 'top',
                at: 'center top+' + (toolbarHeight + dialogOptions.height * dialogPaddingPercent),
                of: window
              };
            }
          }

          dialogOptions.height *= (1 - 2 * dialogPaddingPercent);

          // Get the link title for fallback title.
          if (dialogOptions.title === null || !dialogOptions.title) {
            var linkTitle = $t.attr('title');
            if (linkTitle) {
              dialogOptions.title = linkTitle;
            }
          }

          // Create the wrapper.
          var wrapperHtml = '<div class="paragraphs-previewer-wrapper">' +
            '<div class="paragraphs-previewer-progress"><div class="paragraphs-previewer-progress-bar"></div></div>' +
            '<iframe src="' + url + '" class="paragraphs-previewer-iframe"></iframe>' +
            '</div>';
          var $wrapper = $(wrapperHtml);

          $('.paragraphs-previewer-progress > .paragraphs-previewer-progress-bar', $wrapper).progressbar({
            value: false
          });

          // Initialize the dialog.
          $wrapper.dialog(dialogOptions);
        }
        else {
          alert(Drupal.t('No preview available.'));
        }
      });
    }
  };

})(jQuery, Drupal, this);
