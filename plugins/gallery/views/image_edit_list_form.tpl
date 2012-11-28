{form}
	<table class="list" style="width:100%; margin-bottom:1em;">
		<tr>
			<th></th>
			<th>Name</th>
			<th>File</th>
			<th style="text-align: center;">Order</th>
			<th style="text-align: center;">Delete</th>
		</tr>
		{foreach $images as $image}
			<tr>
				<td><img src="{$image.src}" style="max-height:60px; vertical-align:middle;"/></td>
				<td>{input value="{$image.name}" name="gallery_image_name_{$image.id}"}</td>
				<td><input type="file"/></td>
				<td style="text-align: center;">{input value="{$image.order}" style="width:4em; text-align:center;"}</td>
				<td style="text-align: center;"><input type="checkbox" name="gallery_image_delete_{$image.id}" value="1"/></td>
			</tr>
		{/foreach}
	</table>
	<p style="text-align:right;">{submit value="Update Images" name="gallery_image_edit_list"}</p>
{/form}