#!env ruby
require "YAMl"

yaml = YAML.load(File.read(ARGV[0]))

ARGV[1..-1].each do |arg|
  key, value = arg.split(':')
  yaml = yaml.reject { |exp| exp[key].to_s != value }
end

puts yaml.to_yaml
