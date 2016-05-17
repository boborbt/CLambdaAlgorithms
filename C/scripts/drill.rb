#!env ruby
require "yaml"

def print_usage
  puts <<-EOD
  Usage: drill.rb <timings file> <key/value list>
     where key/value list specifies how to filter the timings data.
     Each key/pair needs to be specified in the format key::value, where
     key is a string and value is a regexp. The program will filter out all
     datapoints that do not match all the given key/value specs.

     Note that quotes may be needed around the key/value list elements to avoid
     shell escaping of special characters.


  Examples:
    drill.rb timings.yml Esercizio::3
      -- returns all the datapoint pertaining to Esercizio 3
    drill.rb timings.yml Esercizio::3 'date::Tue May 17 10:[23].'
      -- returns all the datapoints pertaining to Esercizio 3 and whose date
         is 17/05 at hour 10 and minutes in 20..39.

  EOD
end

if ARGV.size < 2 || ARGV.find { |arg| arg == '-h' }
  print_usage
  exit
end

yaml = YAML.load(File.read(ARGV[0]))

ARGV[1..-1].each do |arg|
  key, value = arg.split('::')
  yaml = yaml.reject { |exp| exp[key].to_s !~ Regexp.new(value) }
end

puts yaml.to_yaml
