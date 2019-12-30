(1..11).each do |elem|
    time = `( time oc test#{elem}/input.txt test#{elem}/output.txt ) 2>&1`
    secs = /real\s+\dm(\d+\.\d+)s/.match(time)[1]
    puts "test#{elem} ran in #{secs} secs"
end

(1..11).each do |elem|
    puts "checking diff for test#{elem}"
    diff = `diff test#{elem}/output.txt test#{elem}/correct.txt`
    if $? != 0
        puts "Diff failed, bailing out -- first 10 lines of diff"
        # puts diff.split("\n")[0..10]
        # exit(1)
    end
end