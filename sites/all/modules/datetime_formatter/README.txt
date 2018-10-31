Date/Time Formatter module.

An extension of the popular Date module which includes a couple of new features:

1. Better formatting for date/time ranges.
Ever wanted to display a specific date range in a more user friendly format? With
the Date module you are limited to something like:

Date format: d/m/Y h:mA
Start date: 01/12/2016 5:30PM
End date 05/12/2016 6:30PM

Result: 01/12/2016 5:30PM - 05/12/2016 6:30PM

What if you could mix and match start- and end date formats:

Date format (single day)     : F d, Y
Date format (multiple days)  : F d - {d}, Y
Date format (multiple months): F d - {F} {d}, Y
Date format (multiple years) : F d, Y - {F} {d}, {Y}
Time format (single)         : h:mA
Time format (multiple)       : h:mA - {h}:{m}{A}

Start date: 01/12/2016 5:30PM
End date 05/12/2016 6:30PM

Date result: January 12 - May 12, 2016
Time result: 5:30PM - 6:30PM

2. We've also added clientside support with moment.js to transform dates and
times into the users local timezone!

3. Timeago is a great jQuery library for displaying fuzzy times.
Highlight certain events that are starting soon, or have already started.

Started 5 min ago.
Starts in 45min.
