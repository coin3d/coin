;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-10-02.

;;; Eval following region

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph (new-socone))
(-> viewer 'show)

;;; End initial eval-region

(-> viewer 'setglrenderaction (new-soglrenderaction (new-sbviewportregion)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(-> viewer 'viewall)
