;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-07.

;;; Eval following region

(define blinker (new-soblinker))
(-> blinker 'addchild (new-socone))
(-> blinker 'addchild (new-sosphere))
(-> blinker 'addchild (new-socylinder))

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph blinker)
(-> viewer 'show)

;;; End initial eval-region


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Test input fields of SoBlinker ndde, play around with these ;;;;;;;;;;;;

(-> (-> blinker 'speed) 'setValue 0.3)
(-> (-> blinker 'whichChild) 'setValue 1)
(-> blinker 'addchild (new-socube))
(-> blinker 'removechild 0)

(begin ; turn off then back on
  (-> (-> blinker 'on) 'setValue 0)
  (-> (-> blinker 'on) 'setValue 1)
  )


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Misc operations on the graph with the SoBlinker ;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Copy the scenegraph.
(define viewer-copy (new-soxtexaminerviewer))
(-> viewer-copy 'show)
(-> viewer-copy 'setscenegraph (-> blinker 'copy 1))

;; Export scenegraph with SoBlinker.
(define writeaction (new-sowriteaction))
(-> writeaction 'apply (-> viewer 'getscenegraph))

;; Read scenegraph with SoBlinker in it.
(let ((buffer "#Inventor V2.1 ascii\n\nSeparator { Blinker { Material { diffuseColor 1 0 0 } Material { diffuseColor 1 1 0 } }  Cube {} }")
      (input (new-soinput)))
  (-> input 'setbuffer (void-cast buffer) (string-length buffer))
  (let ((sceneroot (sodb::readall input)))
    (-> viewer 'setscenegraph sceneroot)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(-> viewer 'viewAll)
(-> viewer 'setscenegraph (new-socube))
(display (-> blinker 'getnumchildren))
