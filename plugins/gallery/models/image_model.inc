<?php

namespace BlankCanvas\gallery;

use BlankCanvas\DataAdapter;

use BlankCanvas\DataSelector;

class Image extends \BlankCanvas\Content {
	
	
	public function __get($key) {
		
		switch($key) {
			case 'filename': return $this->id.'.'.$this->type; break;
			case 'src': return '/data/gallery/images/'.$this->filename; break;
			default: return parent::__get($key);
		}
	}
	
	public function delete() {
		$filePath = 'data/gallery/images/'.$this->filename;
		if(is_file($filePath)) {
			unlink($filePath);
		}
		parent::delete();
	}
	public function getCrumbs() {
		if($parent = $this->getParent()) {
			return array_merge($parent->getCrumbs(), array(
				($this->name ? $this->name : 'Gallery Image') => $this->url
			));	
		}
		return array();
	}
}

?>