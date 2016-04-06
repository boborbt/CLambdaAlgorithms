#!env ruby
require "YAMl"

yaml = YAML.load(File.read(ARGV[0]))

key, value = ARGV[1].split(':')
puts yaml.reject { |exp| exp[key].to_s != value }.to_yaml
