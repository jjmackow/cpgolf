<?php
/**
 * @file
 * Detailed template for the weather module.
 */
?>
<div class="weather well">
  <?php foreach($weather as $place): ?>    
    <?php if (empty($place['forecasts'])): ?>
      <?php print(t('Currently, there is no weather information available.')); ?>
    <?php endif ?>
    <?php foreach($place['forecasts'] as $forecast): ?>
      <div class='weather_day'>
      <div>
        <div class='well cal_date_page'><div>
        <?php print date("<b>D</b><b>M</b><b>jS</b>",strtotime($forecast['formatted_date'])); ?>
        </div></div>
        <?php if (isset($forecast['sun_info'])): ?>
          <br />
          <?php if (is_array($forecast['sun_info'])): ?>
            <div class= "sunrise">
            <?php
              $risetime = date("g:i a",strtotime($forecast['sun_info']['sunrise']));
              print(t('<label>sunrise</label><span>@time</span>', array('@time' => $risetime)));
            ?>
            </div>
            <div class="sunset">
            <?php
              $settime = date("g:i a",strtotime($forecast['sun_info']['sunset']));
              print(t('<label>sunset</label><span>@time</span>', array('@time' => $settime)));
            ?>
            </div>
            <?php
              $daylight = strtotime($forecast['sun_info']['sunset']) - strtotime($forecast['sun_info']['sunrise']);
              print('<div class= "daylight"><label>'.t('daylight'.'</label><span><b>'. intval($daylight/3600).'</b>:<b>' . 60*(($daylight/3600) - intval($daylight/3600)) .'</b></span></span>')); ?></span></div>
          <?php else: ?>
            <?php print($forecast['sun_info']); ?>
          <?php endif ?>
        <?php endif ?>
      </div>
      <div>
        <?php foreach($forecast['time_ranges'] as $time_range => $data): ?>
          <ul>
            <li><?php
              $time_range_start = date("ga", strtotime(substr($time_range, 0, strpos($time_range, '-'))));
              $time_range_end = date("ga", strtotime(substr($time_range, 1+strpos($time_range, '-'))));
              print '<i>'.$time_range_start.'</i>&nbsp;-&nbsp;<i>'.$time_range_end.'</i>'; ?></li>
            <li>
              <?php print $data['symbol']; ?>
            </li>
            <li>
              <?php print $data['temperature']; ?>
              <?php if (isset($data['windchill'])): ?>
                <li>
                <?php print(t('Feels like !temperature', array('!temperature' => $data['windchill']))); ?>
                </li>
              <?php endif ?>
            </li>
            <li>
              <?php print $data['condition']; ?>
            </li>
            <li><?php print $data['wind']; ?></li>
          </ul>
        <?php endforeach; ?>
      </div class='weather_set'>
      </div>
    <?php endforeach; ?>
    <?php if (isset($dace['station'])): ?>
      <p style="clear:left">
        <?php print t('Location of this weather station:'); ?><br />
        <?php print $place['station']; ?>
      </p>
    <?php endif ?>
  <?php endforeach; ?>
</div>
