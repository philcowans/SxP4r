require "sxp4r"

require "date"

line1 = 'ISS (ZARYA)'
line2 = '1 25544U 98067A   10170.35950978  .00009622  00000-0  76728-4 0  3497'
line3 = '2 25544  51.6460 219.4899 0009133 342.2288 127.9785 15.71907726663819'

tle = SxP4r::TLE.new(line1, line2, line3)


puts "Timestamp: #{tle.get_fractional_day}"

target_time = Time.now

puts "Calculating position #{target_time}"

puts "start of year is #{Time.utc(target_time.year)}"

# WHY IS THIS OFF BY ONE?

target_fractional_days = (target_time - Time.utc(target_time.year)) / (24*60*60).to_f + 1

offset = (target_fractional_days - tle.get_fractional_day)*60*24

orbit = SxP4r::Orbit.new(tle)

puts "Need to add #{offset} minutes"

eci = orbit.get_position(offset)

coord_geo = eci.to_geo

puts "Lat: #{coord_geo.lat * 180 / Math::PI}"
puts "Lon: #{coord_geo.lon * 180 / Math::PI}"
puts "Alt: #{coord_geo.alt}"

