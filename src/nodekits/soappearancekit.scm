;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-10-03.

;;; Eval following region

(define root (new-sogroup))
(define appearancekit (new-soappearancekit))
(-> root 'addchild appearancekit)
(-> root 'addchild (new-socone))

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph root)
(-> viewer 'show)

;;; End initial eval-region

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define material (somaterial-cast (-> appearancekit 'getpart (new-sbname "material") 1)))
(-> (-> material 'diffusecolor) 'setvalue (new-sbvec3f 0.8 0.4 0.4))


;; Export scenegraph with kit.
(define writeaction (new-sowriteaction))
(-> writeaction 'apply root)
