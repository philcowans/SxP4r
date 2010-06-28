# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'sxp4r'

# The destination
dir_config(extension_name)

$LIBS += " -lstdc++"

# Do the work
create_makefile(extension_name)
