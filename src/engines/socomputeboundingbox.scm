;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-22.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-22.

;;; Eval following region

;; Read scenegraph with engine in it.
(define root
  (let ((buffer "#Inventor V2.1 ascii\n\n\
Separator { \
  Separator { \
    DEF cone Cone { bottomRadius 2.0  height 6.0 } \
  } \
  DEF spheresep Separator { \
    DEF spheretrans Transform { translation 5 5 0  scaleFactor 0.8 1.5 8.0 } \
    Sphere { } \
  } \
  Separator { \
    Translation { translation 10 0 0 } \
    DEF bboxtext SoText3 { string \"hepp\" } \
  } \
}")
        (input (new-soinput)))
    (-> input 'setbuffer (void-cast buffer) (string-length buffer))
    (sodb::readall input)))

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph root)
(-> viewer 'show)

;;; End initial eval-region

;; Pick out nodes we'll be working with.
(define spheresep (sobase::getnamedbase (new-sbname "spheresep")
                                        (soseparator::getclasstypeid)))
(define text (sotext3-cast (sobase::getnamedbase (new-sbname "bboxtext")
                                                 (sotext3::getclasstypeid))))

;; Make and connect SoComputeBoundingBox engine to the Sphere node.
(define computebbox (new-socomputeboundingbox))
(-> (-> computebbox 'node) 'setvalue (sonode-cast spheresep))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Test input fields of SoComputeBoundingBox engine, play around ;;;;;;;;;;

(-> (-> text 'string) 'connectfrom (-> computebbox 'min))
(-> (-> text 'string) 'connectfrom (-> computebbox 'max))
(-> (-> text 'string) 'connectfrom (-> computebbox 'boxcenter))
(-> (-> text 'string) 'connectfrom (-> computebbox 'objectcenter))

;; Find path to cone.
(define conepath
  (let ((searchaction (new-sosearchaction)))
    (-> searchaction 'setname (new-sbname "cone"))
    (-> searchaction 'apply root)
    (-> searchaction 'getpath)))

;; Connect SoComputeBoundingBox engine to path.
(-> (-> computebbox 'path) 'setvalue conepath)



;; Copy the scenegraph.
(define viewer-copy (new-soxtexaminerviewer))
(-> viewer-copy 'setscenegraph (-> (-> viewer 'getscenegraph) 'copy 1))
(-> viewer-copy 'show)

;; Export scenegraph with engine.
(define writeaction (new-sowriteaction))
(-> writeaction 'apply (-> viewer 'getscenegraph))

;; Read scenegraph with engine in it.
(let ((buffer "#Inventor V2.1 ascii\n\n Text3 { string \"\" = ComposeVec2f { x [ 0, 1 ]  y [ 2, 3] } . vector }")
      (input (new-soinput)))
  (-> input 'setbuffer (void-cast buffer) (string-length buffer))
  (let ((sceneroot (sodb::readall input)))
    (-> viewer 'setscenegraph sceneroot)
    (-> viewer 'viewall)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Confirmed and potential bugs. ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Fixed bugs and false alarms (ex-possible bugs) ;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(-> (-> text 'justification) 'setValue SoText3::CENTER)
(-> (-> text 'parts) 'setValue SoText3::ALL)
(-> (-> text 'string) 'disconnect)
(-> viewer 'viewAll)
