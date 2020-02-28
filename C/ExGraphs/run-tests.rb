require 'timeout'
require 'ansi_colors'

exec_name = ARGV[0] || "oc"
puts "Testing executable:" + exec_name.ansi_yellow.ansi_bold

test_dirs = (1..11).to_a.map { |index| "test#{index}" }

# cleaning up

test_dirs.each do |test_dir|
    `rm -f #{test_dir}/output.txt`
end

# testing times

test_dirs.each do |test_dir|
    start = Time.now

    print "time for " + ("%8s" % [test_dir]).ansi_bold + ": " 

    pid = Process.spawn("cat #{test_dir}/input.txt | #{exec_name} > #{test_dir}/output.txt", :pgroup => true )
    begin
        Timeout.timeout(2) do 
            Process.waitpid(pid)
        end
        finish = Time.now
    rescue
        Process.kill(9, -Process.getpgid(pid))
        puts "Fail".ansi_red.ansi_bold + " (" + "more than 2 seconds".ansi_yellow.ansi_bold + ")"
        next
    end
    
    puts "Ok".ansi_green.ansi_bold + " (" + ("%2.5f" % [finish - start]).ansi_yellow.ansi_bold + " secs)"
end

# testing diffs

test_dirs.each do |test_dir|
    print "diff for " + ("%8s" % [test_dir]).ansi_bold + ": " 

    diff = `diff #{test_dir}/output.txt #{test_dir}/correct.txt`
    if $? != 0
        puts "Fail".ansi_red.ansi_bold 
        puts "\tfirst 10 lines of diff:"

        puts diff.split("\n")[0..10].map { |x| "\t" + x}
    else
        puts "Ok".ansi_green.ansi_bold
    end
end