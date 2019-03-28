#!/usr/bin/ruby
require 'mqtt'
require 'json'
require 'base64'
require 'yaml'
require 'net/https'

# Read in config
settings = nil
if ARGV.length == 1
  settings = YAML.load_file(ARGV[0])
else
  puts "No configuration provided."
  exit
end

# Find the audio files
encouragement_dir = "./audio/encourage/"
congratulations_dir = "./audio/congratulate/"
encouragement = []
congratulations = []
Dir.foreach(encouragement_dir) do |f|
  if File.extname(f) == ".wav"
    encouragement.push(encouragement_dir+f)
  end
end
Dir.foreach(congratulations_dir) do |f|
  if File.extname(f) == ".wav"
    congratulations.push(congratulations_dir+f)
  end
end

# Keep track of the extremities of values reported
highest = [0,0]
lowest = [123456,123456]

# TODO Read this in from the config file
thresholds = [10, 10]

# Digital version of readings, and last version too
digital = []
last_digital = [false, false]
step_count = 5
last_steps = []
step_count.times { |i| last_steps.unshift(i*2000) }

desired_cadence = 100
encouragement_cadence = 80
reached_cadence = false

MQTT::Client.connect(settings['mqtt']['url']) do |c|
  c.get(settings['mqtt']['topic']) do |topic, message|
    #puts "#{topic}: #{message}"
    readings = message.split(',').map { |x| x.to_i }
    if readings[0] > highest[0]
      highest[0] = readings[0]
    end
    if readings[0] < lowest[0]
      lowest[0] = readings[0]
    end
    if readings[1] > highest[1]
      highest[1] = readings[1]
    end
    if readings[1] < lowest[1]
      lowest[1] = readings[1]
    end
    digital[0] = readings[0] > thresholds[0]
    digital[1] = readings[1] > thresholds[1]
    rising = " "#false
    if digital[0] and !last_digital[0]
      rising = "***" #true
      # Record when this step happened
      last_steps.pop # get rid of oldest timestamp
      last_steps.unshift(Time.now.to_f) # store latest
    end
    # Work out cadence from the time the last (step_count-1) steps took
    cadence = (step_count-1)*60/(last_steps.first - last_steps.last)
    # Look for reaching cycling speed
    if cadence > desired_cadence and !reached_cadence
      reached_cadence = true
      puts "####################### WELL DONE!"
      # Congratulate the rider
      Process.spawn("aplay #{congratulations.sample}")
    end
    # See if they've slowed down too much
    if cadence < encouragement_cadence and reached_cadence
      reached_cadence = false
      # Encourage them to cycle faster
      Process.spawn("aplay #{encouragement.sample}")
    end
      
    last_digital[0] = digital[0]
    last_digital[1] = digital[1]
    puts "#{rising}\t#{cadence}\t#{readings[0]}\t#{digital[0]}\t#{readings[1]}\t#{digital[1]}\t\t0: #{lowest[0]}-#{highest[0]},\t1: #{lowest[1]}-#{highest[1]}"
  end
end

