calls = {}

$stdin.each do |line|
  puts line
  match = /Mem_(.*) ptr: (0x.*) file: (.+) line (\d+)/.match(line)
  next if !match

  fun,ptr,file,line = match[1..-1]

  calls[ptr] ||= []
  calls[ptr] << { fun: fun, ptr: ptr, file: file, line: line }
end

calls.each do |key,value|
  count = value.inject(0) do |sum,rec|
    inc = 0
    inc = +1 if ['alloc', 'realloc', 'calloc', 'strdup'].include?(rec[:fun])
    inc = -1 if ['free'].include?(rec[:fun])
    raise "function not known #{rec[:fun]}" if inc == 0

    sum + inc
  end

  if count != 0
    puts "Found #{value.size} elems for ptr #{key}:"
    puts value
    puts "---"
  end

end
