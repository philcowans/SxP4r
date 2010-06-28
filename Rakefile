require 'rubygems'
require 'hoe'
gem 'rake-compiler', '>= 0.4.1'
require "rake/extensiontask"

HOE = Hoe.spec 'sxp4r' do
  developer('Phil Cowans', 'phil@philcowans.com')
  self.version = '0.0.1'
  self.summary = 'sxp4r numerically simulates orbital dynamics and provides utility routines for processing NORAD TLE orbital descriptions.'
  self.url = 'http://github.com/philcowans/SxP4r'
  self.spec_extras = { :extensions => ['ext/SxP4r/extconf.rb'] }
end

RET = Rake::ExtensionTask.new("sxp4r", HOE.spec) do |ext|
  ext.lib_dir = 'lib'
  ext.config_options << ENV['EXTOPTS']
end

Rake::Task[:test].prerequisites << :compile
