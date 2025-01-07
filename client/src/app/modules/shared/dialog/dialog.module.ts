import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import {DialogComponent} from "./dialog.component";
import {DialogActionPanelComponent} from "./dialog-action-panel/dialog-action-panel.component";
import {ActionButtonModule} from "../action-button/action-button.module";
import {MatButtonModule} from "@angular/material/button";
import {MatDialogModule} from "@angular/material/dialog";
import {ValidationMessageComponent} from "../validation-message/validation-message.component";



@NgModule({
  declarations: [
    DialogComponent,
    DialogActionPanelComponent
  ],
    imports: [
        CommonModule,
        ActionButtonModule,
        MatButtonModule,
        MatDialogModule,
        ValidationMessageComponent
    ]
})
export class DialogModule { }
