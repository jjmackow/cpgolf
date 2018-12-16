jQuery(document).ready(function ($) {  
  /**
   * Creating new quicktabs animation mimicking parallax slider
   * by adding new click event handler
   * if needed
   * add auto slide with timer
   */
   
  // double check the class and change if needed
  // this is the quicktabs per content wrapper
  var quicktabsPage = $('.quicktabs-tabpage');
 
  if (quicktabsPage.length != 0) {
    var totalElems = quicktabsPage.length,
        quicktabsMain = $('.quicktabs-wrapper'), // this is the main wrapper for all quick tabs element
        quicktabsSlideWrapper = $('.quicktabs_main'),  // this is the wrapper for all quicktabs per content wrapper a.k.a $('.quicktabs-tabpage') wrapper
        quicktabTabs = quicktabsMain.find('.quicktabs-tabs'), // this is the tabs navigation wrapper
        tabsHeight = quicktabTabs.outerHeight(true);
     
    var setHeight = function (slideIndex) {
      var pageHeight = 1.2*(quicktabsPage.eq(slideIndex).outerHeight(true) + tabsHeight);
      quicktabsMain.stop().animate({height : pageHeight}, '400');
    }
     
    var setWidth = function() {
      var frameWidth = quicktabsMain.outerWidth(true);
      var totalwidth = frameWidth * totalElems;
      quicktabsSlideWrapper.width(totalwidth);
      quicktabsPage.width(frameWidth);    
    }
     
    var slideTo = function (slideIndex) {
      var w_w = quicktabsMain.outerWidth(true);
      var slide_to  = parseInt(-w_w * slideIndex);
       
      quicktabsSlideWrapper.stop().animate({
        left  : slide_to + 'px'
      },800, 'easeOutQuad');
    }
       
    // start the slider by setting the correct width and height
    setHeight(0, quicktabsPage, quicktabsSlideWrapper);
    setWidth(quicktabsMain, totalElems);
     
    // binding the click
    $('ul.quicktabs-tabs li').each(function(){
      var slideIndex = $('ul.quicktabs-tabs li').index($(this));
      // bind a the new override sliding method
      $(this).find('a').click(function(e) {
        /**
         * This is the area where the animation coding should take place
         */
        e.preventDefault();
        setHeight(slideIndex);
        setWidth();
        slideTo(slideIndex);    
      });
    });
     
    // resize the width upon windows resizing
    $(window).resize(function(){
      setWidth();
    });
  }
});