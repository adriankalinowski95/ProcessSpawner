import { Component, Input } from '@angular/core';
import { MenuItem } from '../../models/menu-item';
import { AuthService } from '../../../../../auth/services/auth.service';

@Component({
  selector: 'app-sidenav',
  templateUrl: './sidenav.component.html',
  styleUrl: './sidenav.component.scss'
})
export class SidenavComponent {
    @Input() isExpanded: boolean = false;

    menuItems: MenuItem[] = [
        {
            label: 'Dashboard',
            icon: 'dashboard',
            route: '/dashboard'
        },
        {
            label: 'Process Spawner',
            icon: 'rocket_launch',
            route: '/process-spawner'
        },
        {
            label: 'Process Manager',
            icon: 'manage_accounts',
            route: '/process-manager'
        }
    ];

    constructor(public authService: AuthService) {}
}
