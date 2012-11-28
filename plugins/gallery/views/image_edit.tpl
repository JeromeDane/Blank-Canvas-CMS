<h1>{if $image.id}Edit{else}Create{/if} Image</h1>

{view context="gallery" controller="image" action="edit_form" id=$id}
