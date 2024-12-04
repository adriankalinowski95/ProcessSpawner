import { Component, EventEmitter, Output } from '@angular/core';
import { AuthService } from '../../../auth/services/auth.service';

@Component({
  selector: 'app-toolbar',
  templateUrl: './toolbar.component.html',
  styleUrl: './toolbar.component.scss'
})
export class ToolbarComponent {
    private isExpanded: boolean = false;

    @Output()
    isExpandedEvent = new EventEmitter<boolean>();

    constructor(private authService: AuthService) {}
    
    logout() {
        this.authService.logout();
    }
    
    toggleSubMenuButton() {
        this.isExpanded = !this.isExpanded;

        this.isExpandedEvent.emit(this.isExpanded);
    }
}
