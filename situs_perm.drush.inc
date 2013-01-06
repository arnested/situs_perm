<?php

/**
 * @file
 * Situs permission plugin
 */

/**
 * Implements hook_situs_pre_build().
 */
function situs_perm_situs_pre_build() {
  $failed = FALSE;

  $loglevel = drush_get_option('situs-perm-loglevel', 'error');
  $group = drush_get_option('situs-perm-chgrp', FALSE);
  $perm = drush_get_option('situs-perm-chmod', 02750);
  $umask = drush_get_option('situs-perm-umask', 0027);

  // Get the build_path where Situs will make the new build. Strip of the last
  // '/build' part of the path since that has not been created yet.
  $build_path = substr(drush_get_context('SITUS_BUILD_PATH'), 0, -6);

  // If no group has been supplied try to make a good guess based on the groups
  // the current user belongs to and which are known to be groups belonging to
  // web servers.
  if (!$group) {
    $username = get_current_user();
    $groups = explode(' ', trim(`id -Gn $username`, "\n"));
    $group = array_pop(array_intersect($groups, array('_www', 'www-data')));
  }

  // If we found a group let us try to chgrp the build path to it.
  if (is_string($group)) {
    if (!@chgrp($build_path, $group)) {
      $failed = !drush_log('Could not change build path to group: ' . $group, $loglevel) || $failed;
    };
  }
  else {
    $failed = !drush_log('Could not find a suitable web server group.', $loglevel) || $failed;
  }

  // Let us try to chmod the build path.
  if (!@chmod($build_path, $perm)) {
    $failed = drush_log('Could not change permissions to ' .  $perm, $loglevel) || $failed;
  };

  // Set umask.
  umask($umask);

  // Return success or failure.
  return !$failed;
}

/**
 * Implements hook_situs_hooks().
 */
function situs_perm_situs_plugin() {
  return array(
    'situs-perm' => array(
      'name' => 'Situs permissions',
      'description' => 'Fixes permissions.',
      'pre_build' => 'situs_perm_situs_pre_build',
    ),
  );
}
