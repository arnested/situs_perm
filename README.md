# Situs permission plugin

This plugin for [Situs](https://github.com/xendk/situs) will make
your build with file system permissions set appropriately.

The plugin will basically do

 * `chgrp _www-data $build_path`
 * `chmod 2750 $build_path`
 * Sets umask to 0027

You need to change group and permission on your `files`folder once and
for all (situs will preserve the MYSITE folder during builds):

    sudo chown -R $(id -un):_www sites/MY_SITE
    sudo chmod -R u+rwX,g+rXs sites/MY_SITE
    sudo chmod -R u+rwX,g+rwXs sites/MY_SITE/files

@todo: if this is a first build there is no site - what can we do to
get it right in this case?

## Add your user to the _www group on Mac OS X

    sudo dseditgroup -o edit -a $(id -un) -t user _www


## Add your user to the www-data group on Linux

    sudo adduser $(id -un) www-data

## Options

In your drushrc.php (or site alias) you can set the following options:

 * `situs-perm-chgrp`: The group to change the $build_path
   to. If not set a good guess at the right group will be made.
 * `situs-perm-chmod`: The file permission to set on the $build_path
   folder. Defaults to `02750`. Must be octal (preceding `0`).
 * `situs-perm-umask`: The umask to use for the build
   process. Defaults to `0027`. Must be octal (preceding `0`).
 * `situs-perm-log-level`: Which level to log errors to. Defaults to
   "error". If you don't mind if setting permissions failed and don't
   want it to abort your build process, set it to "warning".
