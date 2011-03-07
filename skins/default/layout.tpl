<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title> - </title>
<meta name="description" content="" />
<meta name="keywords" content="web,web design,web development,web hosting,web applications,design,development,content management,cms,content management systems,shopping cart,ecommerce,e-commerce,search engine optimization,seo,html,css" />
<link rel="icon" href="/favicon.ico" type="image/x-icon"/>
<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon"/>
<script type="text/javascript" src="/system/javascript/common.js"></script>
</head>

<body>
<div id="outter_wrapper">
	<div id="inner_wrapper">
		<!--<img src="/skins/default/images/blank_canvas.jpg" style="float:right; border-left:5px solid #fff;" />-->
		<!--<img src="/skins/default/images/logo_100px.jpg" style="float:right; border-left:5px solid #fff;" />-->
		<div id="header">
			<a href="/"><img src="/skins/default/images/logo_100px.jpg" style="float:left; margin-right:10px;" align="Logo" /></a>
			<div style="width:400px; float:right; margin-right:20px; text-align:right;">
			</div>
			<h1>Blank Canvas</h1>
			<h2>Web development, applications, &amp; resources</h2>
		</div>
		<div id="content" style="width:750px; float:left;">
			{$system.controller} - {$system.action}
			<div style="margin:15px">
				{$system.body}
			</div>
		</div>
		<div style="margin-left:750px;">
			<div style=" margin:10px 20px 20px 0;">
				<div class="right_box">
					<p><strong>Recent Projects:</strong></p>
					<div align="center">
					</div>
				</div>
				<div class="right_box"></div>
				<div class="right_box">
					<p><strong><a href="/forums/latest/">Latest Forum Posts:</a></strong></p>
				</div>
			</div>
		</div>
		<div id="footer">
			&copy; 2007-{$smarty.now|date_format:'%Y'} Jerome Dane - Blank Canvas Productions, LLC
		</div>
	</div>
	<!--
	<script type="text/javascript">
	google_ad_client = "pub-5123499597920728";
	/* 120x600, created 11/24/08 */
	google_ad_slot = "5844220188";
	google_ad_width = 120;
	google_ad_height = 600;
	
	</script>
	<script type="text/javascript"
	src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
	</script>-->
</div>
<!-- =============================================== Google Analutics ======================== -->
<script type="text/javascript">
	var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
	document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
	var pageTracker = _gat._getTracker("UA-3650415-1");
	pageTracker._initData();
	pageTracker._trackPageview();
</script>

<!-- ====================================================================== Loading message ====================== -->
<div id="loading_message" style="display:none; position:absolute; top:0; left:0; z-index:999999">
	<div id="loading_message_box" class="popup_form" style="width:230px; padding:20px;">
		<h1 style="margin:0; font-size:16px;">
			<img src="/skins/default/images/loading.gif" alt="Loading... please wait." align="absmiddle" style="margin-right:10px;" />
			Loading... Please Wait
		</h1>
	</div>
	<div id="loading_message_mask" class="screen_mask"></div>
</div>


</body>
</html>
