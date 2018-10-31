/**
 * @file
 * Goomap javascript.
 */

(function($) {
  Drupal.behaviors.goomap = {
    attach : function(context, settings){
      for (var id in settings.goomap) {
        $('#' + id, context).once('goomap-processed', function() {
          Drupal.goomap.createGoomap(id);
        });
      }
    }
  };
  
  Drupal.goomap = {};
  Drupal.goomap.createGoomap = function(id) {
    $('#' + id).trigger('goomap.beforeCreation', [id]);
    if(Drupal.settings.goomap[id].mapCreate){
      try {
        Drupal.settings.goomap[id].map = new google.maps.Map(
            document.getElementById(id),
            Drupal.settings.goomap[id].mapOptionsJson
        );
        Drupal.goomap.createMarkers(id);
        if (Drupal.settings.goomap[id].centerMarkers) {
          Drupal.goomap.zoomAndFit(id);
        }
      } catch (error) {
        console.log(error);
      }
    }
  };
  
  //Create markers.
  Drupal.goomap.createMarkers = function(id) {
    var infowindow = new google.maps.InfoWindow({
      content : ""
    });
    Drupal.settings.goomap[id].items
    for(var i in Drupal.settings.goomap[id].items){
      var item = Drupal.settings.goomap[id].items[i];
      var markerOptions = item.markerOptions;
      var pos = new google.maps.LatLng(
          parseFloat(item.geodata.lat),
          parseFloat(item.geodata.lon)
      );
      markerOptions.position = pos;
      markerOptions.map = Drupal.settings.goomap[id].map;
      var marker = new google.maps.Marker(markerOptions);
      item.marker = marker;
      
      
      if (typeof item.infowindowContent != 'undefined' &&
          item.infowindowContent != '') {
        marker.html = item.infowindowContent;
        google.maps.event.addListener(marker, 'click', function() {
          infowindow.setContent(this.html);
          infowindow.open(marker.map, this);
        });
      }
      
    }
    $('#' + id).trigger('goomap.markersCreated', [id]);
  };
  //Remove markers from map (set map to null).
  Drupal.goomap.clearMarkers = function(id) {
    var settings = Drupal.settings.goomap[id];
    for (var i in settings.items) {
      try{
        settings.items[i].marker.setMap(null);
      }catch(error){}
    }
    $('#' + id).trigger('goomap.markersCleared', [id]);
  };
  //Delete markers/items.
  Drupal.goomap.deleteMarkers = function(id) {
    Drupal.goomap.clearMarkers(id);
    Drupal.settings.goomap[id].items = [];
    $('#' + id).trigger('goomap.markersDeleted', [id]);
  }
  //Zoom to fit all visible markers, taking into account the centerZoomLevel setting and mapOptions.maxZoom.
  Drupal.goomap.zoomAndFit = function(id) {
    var settings = Drupal.settings.goomap[id];
    var latlngbounds = new google.maps.LatLngBounds();
    for (var i in settings.items) {
      var pos = new google.maps.LatLng(
          parseFloat(settings.items[i].geodata.lat),
          parseFloat(settings.items[i].geodata.lon));
      latlngbounds.extend(pos);
    }
    google.maps.event.addListenerOnce(settings.map, 'bounds_changed', function(event) {
      var centerZoomLevel = parseInt(settings.centerZoomLevel);
      var mapZoomNow = settings.map.getZoom();
      if (centerZoomLevel) {
        if (mapZoomNow > centerZoomLevel) {
          settings.map.setZoom(centerZoomLevel);
        }
      }
    });
    settings.map.fitBounds(latlngbounds);
    $('#' + id).trigger('goomap.zoomToFit', [id]);
  };
  // Add the ajax commands.
  if (typeof Drupal.ajax != 'undefined') {
    Drupal.ajax.prototype.commands.goomapClearMarkers = function(ajax, response, status) {
      Drupal.goomap.clearMarkers(response.id);
      $('#' + response.id).trigger('goomap.ajaxUpdated', [response.id, 'clearMarkers']);
    }
    Drupal.ajax.prototype.commands.goomapDeleteMarkers = function(ajax, response, status) {
      Drupal.goomap.deleteMarkers(response.id);
      $('#' + response.id).trigger('goomap.ajaxUpdated', [response.id, 'deleteMarkers']);
    }
    Drupal.ajax.prototype.commands.goomapNewMarkers = function(ajax, response, status) {
      Drupal.settings.goomap[response.id].items = response.newMarkers;
      $('#' + response.id).trigger('goomap.ajaxUpdated', [response.id, 'newMarkers']);
    }
    Drupal.ajax.prototype.commands.goomapCreateMarkers = function(ajax, response, status) {
      Drupal.goomap.createMarkers(response.id);
      $('#' + response.id).trigger('goomap.ajaxUpdated', [response.id, 'createMarkers']);
    }
    Drupal.ajax.prototype.commands.goomapZoomAndFit = function(ajax, response, status) {
      Drupal.goomap.zoomAndFit(response.id);
      $('#' + response.id).trigger('goomap.ajaxUpdated', [response.id, 'zoomAndFit']);
    }
  }
})(jQuery);
