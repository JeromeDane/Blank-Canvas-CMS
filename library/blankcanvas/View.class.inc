<?php

namespace BlankCanvas;

class View extends \Smarty {
	
	protected $_controllerName;
	protected $_layoutFile = 'skins/default/layout.tpl';
	
	public function __construct($controllerName) {
		parent::__construct();
		$this->_controllerName = $controllerName;
	}
	
	/**
	 * @return View
	 */
	public static function get404() {
		require_once 'application/controllers/filenotfound.controller.inc';
		$controller = new FileNotFoundController();
		return $controller->view;
	}
	
	
	public function render() {
		$traceStack = debug_backtrace();
		$actionName = $traceStack[1]['function'];
		
		$viewsFolder = 'application/views/';
		$filePath = $viewsFolder.$this->_controllerName.'/'.$actionName.'.tpl';
		if(is_file($filePath)) {
			
			$systemTags = array(
				'controller' => $this->_controllerName,
				'action' => $actionName,
				'version' => App::getVersion()
			);
			$this->assign(array(
				'system' => $systemTags
			));
			
			$renderedString = $this->fetch($filePath);
			
			if(!is_null($this->_layoutFile)) {
				
				App::addStylesheet('skins/default/skin.css');
				
				$systemTags['body'] = $renderedString;
				
				$layout = new \Smarty();
				$layout->assign(array(
					'system' => $systemTags
				));
				return $this->_addStylesToSkinHtml($layout->fetch($this->_layoutFile));
			} else {
				return $renderedString;
			}
		} else {
			// throw new Exception('View error: can\'t find template file "'.$filePath.'"');
			require_once 'application/controllers/filenotfound.controller.inc';
			$controller = new FileNotFoundController();
			return $controller->index();
		}
	}
	protected static function _addStylesToSkinHtml($inputHtml) {
		$style = '';
		$links = '';
		foreach (App::getStylesheetPaths() as $src => $trash) {
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