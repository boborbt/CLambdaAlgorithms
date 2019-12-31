require 'timeout'
require 'ansi_colors'

(1..11).each do |elem|
    start = Time.now

    print "time for " + "test#{elem}".ansi_bold + ":" 

    begin
        Timeout.timeout(2) do 
            `oc test#{elem}/input.txt test#{elem}/output.txt`
        end
        finish = Time.now
    rescue
        puts " more than 2 seconds".ansi_yellow.ansi_bold + " -- " + "Fail".ansi_red.ansi_bold
        next
    end
    
    puts " #{finish - start}".ansi_yellow.ansi_bold + " secs" + " -- " + "Ok".ansi_green.ansi_bold
end

(1..11).each do |elem|
    print "diff for " + "test#{elem}".ansi_bold + ": "
    diff = `diff test#{elem}/output.txt test#{elem}/correct.txt`
    if $? != 0
        puts "Diff failed -- first 10 lines of diff".ansi_red.ansi_bold

        # puts diff.split("\n")[0..10]
        # exit(1)
    else
        puts "Ok".ansi_green.ansi_bold
    end
end