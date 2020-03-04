
HELP_TEXT = """
This script tests the performances of a solution to 
exercise 4. The scripts assumes:

  - that it is run on the directory containing the folders with
    the inputs/outputs provided as tests (i.e., it assumes that
    it there exists test1, test2, ..., test11 subdirs in the current
    directory).
  - that the program implementing the solution is called 'oc', but you
    may specify another name as the first parameter to the script
  - that the program implementing the solution takes two arguments:
         - the name of the input file
         - the name of the output file
    and behaves accordingly (i.e., reads from the input file and writes
    onto the output file).

For each test directory the script runs `oc`, checks if the solution is
correct and reports the performances. If the solution is *not* correct
it also reports the first few lines of the diff between the correct 
and the calculated solutions.

This script requires the 'ansi_colors' ruby gem. You can install it
with the command: `gem install ansi_colors`.

If everything is ok, you should get a colored output containing the 
the following contents (assuming the solution solves correctly all
tests in less than two seconds):

Testing executable:bin/oc
time for    test1: Ok (0.37711 secs)
time for    test2: Ok (0.14862 secs)
time for    test3: Ok (0.09273 secs)
time for    test4: Ok (0.09057 secs)
time for    test5: Ok (0.07473 secs)
time for    test6: Ok (0.33535 secs)
time for    test7: Ok (0.18457 secs)
time for    test8: Ok (0.23440 secs)
time for    test9: Ok (0.33470 secs)
time for   test10: Ok (0.33108 secs)
time for   test11: Ok (0.35056 secs)
diff for    test1: Ok
diff for    test2: Ok
diff for    test3: Ok
diff for    test4: Ok
diff for    test5: Ok
diff for    test6: Ok
diff for    test7: Ok
diff for    test8: Ok
diff for    test9: Ok
diff for   test10: Ok
diff for   test11: Ok

"""

require 'timeout'
require 'ansi_colors'

if ARGV.size == 1 && ARGV[0] == "-h" || ARGV[0] == '--help'
    puts HELP_TEXT
    puts "Usage: run-tests.rb [-h] [<program name>]"
    exit(0)
end

exec_name = ARGV[0] || "bin/oc"
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
