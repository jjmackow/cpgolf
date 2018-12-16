# Paragraphs Previewer

Provides a rendered preview of a paragraphs item while on an entity form.

# Features

* Preview the rendered paragraph before saving the entity.
* Previewer can be enabled per field instance.
* Full width window to preview the design.
* Resizable window to preview responsive designs.

# Caveats

* Preview popup uses the front end theme to style the rendered markup.  This
  assumes that all styling is applied to that paragraph's markup and does not
  need any other page context / wrapping markup, example node markup.


# Installation

* Install as usual, see https://drupal.org/node/895232 for further information.
* Navigate to Administer >> Module: Enable "Paragraphs Previewer".
* Create / Edit a paragraphs field:
  * Set widget to "Embedded with Previewer"
  * Set "Default edit mode" to "Closed" or "Preview".

# Requirements

* Paragraphs module (https://www.drupal.org/project/paragraphs) with the
  following patches:
  * https://www.drupal.org/node/2564327: Patch #85 - Adds host entity info to
    the item's element array. This provides the host entity needed to render
    the paragraph from the form state.
  * https://www.drupal.org/node/2649608: Allow access by default for paragraphs
    with no hosts.  This is needed to render new paragraphs and nested new
    paragraphs from the form state when the paragraphs module cannot determine
    the host.

# Optional Support
  * https://www.drupal.org/node/2201251: Drupal 7 core patch to fix
    field_language's static cache collisions for multiple new entities of the
    same entity type but different bundle. This patch could be needed depending
    on the field formatter that is used to render the paragraphs field.
