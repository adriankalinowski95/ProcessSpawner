import {Component, ComponentRef, inject, OnInit, viewChild, ViewContainerRef} from '@angular/core';
import {MAT_DIALOG_DATA, MatDialog, MatDialogContainer} from "@angular/material/dialog";
import {shared} from "../shared";
import ActionType = shared.action_button.models.ActionType;
import Config = shared.action_button.models.Config;
import {catchError, Observable, of, take, tap} from 'rxjs';
import { DialogComponentRef } from './dialog-component-ref';

interface CustomDialogConfig {
}

@Component({
  selector: 'app-dialog',
  templateUrl: './dialog.component.html',
  styleUrl: './dialog.component.scss'
})
export class DialogComponent<T extends DialogComponentRef> implements OnInit {
  readonly dialog = inject<any>(MatDialog);
  readonly container = inject<any>(MatDialogContainer);
  readonly data = inject<any>(MAT_DIALOG_DATA);

  validationErrors: [] = [];

  private componentRef: ComponentRef<T>

  viewContainerRef = viewChild('dialogContent', {read: ViewContainerRef})
  actionPanelButtons: Config[];
  ngOnInit() {
    this.actionPanelButtons = this.data.config.actionPanelButtons;
    if(this.viewContainerRef()) {
      this.componentRef = (this.viewContainerRef() as ViewContainerRef).createComponent(this.data.contentRef);
      this.componentRef.instance.data = this.data.contentData;
    }
  }

  onActionButtonClick($event: ActionType) {
    let id = this.container._config.id;
    if ($event === ActionType.Save) {
      this.validationErrors = [];
      //add validation type to config content dialog component
      if (this.componentRef.instance.validate instanceof Function) {
        try {
          this.componentRef.instance.validate() && id && this.dialog.getDialogById(id).close(this.componentRef.instance.outputData);
        } catch (e: any) {
          const errors = JSON.parse(e);
          this.validationErrors = errors;
        }

      } else if (this.componentRef.instance.validateAsync instanceof Function) {
        try {
          this.componentRef.instance.validateAsync().pipe(
            tap(v => this.dialog.getDialogById(id).close(this.componentRef.instance.outputData)),
            catchError(e => {
              const errors = JSON.parse(e);
              this.validationErrors = errors;
              return of(e);
            })
          ).subscribe();
        } catch (e: any) {
          const errors = JSON.parse(e);
          this.validationErrors = errors;
          console.log(this.validationErrors)
        }

      } else {
        id && this.dialog.getDialogById(id).close(this.componentRef.instance.outputData);
      }
    } else if ($event === ActionType.Close) {
      id && this.dialog.getDialogById(id).close(false);
    }
  }

  onCloseValidationMessage() {
    this.validationErrors = [];
  }
}