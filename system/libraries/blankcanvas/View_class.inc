<?php

namespace BlankCanvas;

class View extends Template {
	
	protected $_contextPath;
	protected $_controllerName;
	protected $_actionName;
	protected $_layoutFile = 'skins/default/layout.tpl';
	
	protected static $_title = '';
	protected static $_cssFilePaths;
	protected static $_crumbs = array();
	
	public function __construct($context, $controllerName = null, $actionName = null) {
		parent::__construct();
		$this->_contextPath = ($context == 'system' ? 'system' : 'plugins/' . $context) . '/';
		$this->_controllerName = $controllerName;
		$this->_actionName = $actionName;
	}
	
	/**
	 * @todo document addStylesheet
	 *
	 */
	public static function linkCssFile($source) {
		if (is_file($source)) {
			self::$_cssFilePaths[$source] = true;
		} else {
			throw new Exception('Can\'t find stylesheet at "' . $source . '"');
		}
	}
	
	/**
	 * @return View
	 */
	public static function get404() {
		require_once BC_ROOT. '/system/controllers/filenotfound_controller.inc';
		$controller = new FileNotFoundController();
		return $controller->index();
	}
	
	public function render($tpl_var = null, $value = null, $nocache = false) {
		if(!is_null($tpl_var)) {
			$this->assign($tpl_var, $value);
		}
		
		$actionName = $this->_getActionName();
		// determine the name of the controller action called
		
		$viewsFolder = $this->_contextPath.'views/';
		if($this->_controllerName == 'index' && $actionName == 'index') {
			$filePath  = $viewsFolder.'index.tpl';
		} else {
			$filePath = $viewsFolder.$this->_controllerName.'_'.$actionName.'.tpl';
		}
		if(is_file($filePath)) {
			
			// check for custom skin file
			if(is_file('skins/default/'.$filePath)) {
				$filePath = 'skins/default/'.$filePath;
			}
			
			// include CSS
			$cssPath = $this->_contextPath . 'css/' . $this->_controllerName . '_' . $actionName . '.css';
			if(is_file($cssPath)) {
				View::linkCssFile($cssPath);
			}
			
			// assign system tags
			$systemTags = array(
				'controller' => $this->_controllerName,
				'action' => $actionName,
				'version' => App::getVersion(),
				'page_title' => self::$_title,
			);
			
			$this->assign(array(
				'system' => $systemTags
			));
			
			set_error_handler(function($errno, $errstr) {
				$bt = debug_backtrace();
				if(isset($bt[0]['file'])) {
					echo("\n\n{" . $errstr . " in ".$bt[0]['file']." line ".$bt[0]['line']."}\n\n");
				} else {
					echo($errstr);
				}
			}, E_ALL);
			$renderedString = $this->fetch($filePath);
			restore_error_handler();
			
			// check for load from smarty partial_view function
			$bt = debug_backtrace();
			if($bt[3]['function'] == 'smarty_function_view' || $bt[4]['function'] == 'smarty_function_view') {
				$this->_layoutFile = null;
			}
			if(!is_null($this->_layoutFile)) {
				
				self::linkCssFile('skins/default/skin.css');
				
				$systemTags['body'] = $renderedString;
				
				$layout = new Template();
				$layout->assign(array(
					'system' => $systemTags
				));
				return $this->_addStylesToSkinHtml($layout->fetch($this->_layoutFile));
			} else {
				return $renderedString;
			}
		} else {
			 throw new Exception('View error: can\'t find template file "'.$filePath.'"');
			die('pr');
			require_once 'system/controllers/filenotfound_controller.inc';
			$controller = new FileNotFoundController();
			return $controller->index();
		}
	}
	public function setActionName($name) {
		$this->_actionName = $name;
	}
	public function setLayoutFile($path) {
		$this->_layoutFile = $path;
	}
	protected function _getActionName() {
		if(!is_null($this->_actionName)) {
			return $this->_actionName;
		} else {
			$bt = debug_backtrace();
			return $bt[2]['function'] == 'view' ? $bt[3]['function'] : $bt[2]['function'];
		}
	}
	
	public static function addCrumbs(array $crumbs) {
		self::$_crumbs = array_merge(self::$_crumbs, $crumbs);
	}
	public static function getCrumbs() {
		return self::$_crumbs;
	}
	public static function setTitle($title) {
		return self::$_title = $title.'';
	}
	
	protected static function _addStylesToSkinHtml($inputHtml) {
		$style = '';
		$links = '';
		foreach (self::$_cssFilePaths as $src => $trash) {
			if (is_file($src)) {
				$styleContents = file_get_contents($src);
				if (App::getRootUri() != '/') {
					$style .= "\n\t" . '<style type="text/css">' . preg_replace('/url\(("|\')?\//i', "url(\$1" . App::getRootUri(), $styleContents) . '</style>' . "\n";
				} else {
					$links .= "\n\t" . '<link rel="stylesheet" type="text/css" href="/' . $src . '" /> ';
				}
			}
		}
		$outputHtml = preg_replace('/<\/head>/i', $style . '</head>', $inputHtml, 1);
		$outputHtml = preg_replace('/<\/head>/i', $links . '</head>', $outputHtml, 1);
		return $outputHtml;
	}
	
}

?>