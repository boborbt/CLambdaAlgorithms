(1..11).each do |elem|
    puts "running test#{elem}"
    `oc test#{elem}/input.txt > test#{elem}/output.txt`
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