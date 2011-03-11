<?php

namespace BlankCanvas;

// @todo document Template class
class Template extends \Smarty {
	
	public function __construct() {

		parent::__construct();
		
		$this->compile_dir = 'cache/compiled_templates';
		$this->cache_dir ='cache/templates';
		
		// escape htmlentities by default
		$this->default_modifiers = array('escape:"htmlall"');

		$appSmartyPluginDir = 'application/libraries/blankcanvas/smarty_plugins/';
		
		if(isset($this->plugins_dir) && is_array($this->plugins_dir)) {
			$this->plugins_dir = array_merge(array($appSmartyPluginDir), $this->plugins_dir);
		} else {
			$this->plugins_dir = array($appSmartyPluginDir);
		}
	}
	
}

?>