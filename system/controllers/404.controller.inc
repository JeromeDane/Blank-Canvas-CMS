<?php

class FileNotFoundController extends BCController {
	
	public function index() {
		BC::setPageTitle("Error 404: File Not Found");
		return BC::view();
	}
	
}

?>