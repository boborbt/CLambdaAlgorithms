require 'timeout'
require 'ansi_colors'

exec_name = ARGV[0] || "oc"
puts "Testing executable:" + exec_name.ansi_yellow.ansi_bold

test_dirs = (1..11).to_a.map { |index| "test#{index}" }

# cleaning up

test_dirs.each do |test_dir|
    `rm #{test_dir}/output.txt`
end

# testing times

test_dirs.each do |test_dir|
    start = Time.now

    print "time for " + "#{test_dir}".ansi_bold + ":" 

    begin
        Timeout.timeout(2) do 
            `cat #{test_dir}/input.txt | #{exec_name} > #{test_dir}/output.txt`
        end
        finish = Time.now
    rescue
        puts " more than 2 seconds".ansi_yellow.ansi_bold + " -- " + "Fail".ansi_red.ansi_bold
        next
    end
    
    puts " #{finish - start}".ansi_yellow.ansi_bold + " secs" + " -- " + "Ok".ansi_green.ansi_bold
end

# testing diffs

test_dirs.each do |test_dir|
    print "diff for " + "#{test_dir}".ansi_bold + ": "
    diff = `diff #{test_dir}/output.txt #{test_dir}/correct.txt`
    if $? != 0
        puts "Fail".ansi_red.ansi_bold + "-- first 10 lines of diff:"

        puts diff.split("\n")[0..10]
        # exit(1)
    else
        puts "Ok".ansi_green.ansi_bold
    end
end