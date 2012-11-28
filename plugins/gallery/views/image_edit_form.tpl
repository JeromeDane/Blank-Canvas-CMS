{form enctype="multipart/form-data" style="display:inline;"}
	<input type="hidden" name="parent" value="{$parent_id}"/>
	Name: {input name="name"} &nbsp;
	File: 
	<input type="file" name="image">

	{submit value="Upload"}
{/form}